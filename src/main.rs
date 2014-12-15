#![feature(phase)]

#[phase(plugin, link)]
extern crate log;
extern crate serialize;

extern crate docopt;
#[phase(plugin)]
extern crate docopt_macros;
extern crate toml;

use std::collections::TreeMap;
use std::io::{File, IoResult};
use std::io::net::udp::UdpSocket;
use std::io::net::ip::{IpAddr, Ipv4Addr, SocketAddr};

docopt!(Args deriving Show, "
Usage: raft [-f FILE]
       raft [-h | --help]

Options:
  -f FILE          Read config from FILE. [default: raft.toml]
  -h, --help       Show this message.
")

fn main() {
    let args: Args = Args::docopt().decode().unwrap_or_else(|e| e.exit());
    let config = read_config(args.flag_f.as_slice());

    let empty_section: toml::Value = toml::Table(TreeMap::new());
    let net_section = config.get("net").unwrap_or(&empty_section);
    let local_port = match net_section.lookup("local_port") {
        Some(&toml::Integer(p)) if p > 0 && p < 65536 => p,
        Some(v) => panic!("Invalid value for net.local_port: {}", v),
        None => 17100
    };
    let multicast_addr = match net_section.lookup("multicast_addr") {
        Some(&toml::String(ref a)) if from_str::<IpAddr>(a.as_slice()).is_some() => {
            from_str::<IpAddr>(a.as_slice()).unwrap()
        },
        Some(v) => panic!("Invalid value for net.multicast_addr: {}", v),
        None => from_str::<IpAddr>("239.10.10.10").unwrap()
    };

    let mut socket = match bind_socket(local_port as u16, multicast_addr) {
        Ok(s) => s,
        Err(e) => panic!("Couldn't bind socket: {}", e)
    };
}

fn bind_socket(port: u16, multicast_addr: IpAddr) -> IoResult<UdpSocket> {
    debug!("Using local port {}", port);
    let mut socket = try!(UdpSocket::bind(SocketAddr { ip: Ipv4Addr(0, 0, 0, 0), port: port }));
    debug!("Joining multicast group {}", multicast_addr);
    try!(socket.join_multicast(multicast_addr));
    Ok(socket)
}

fn read_config(filename: &str) -> toml::TomlTable {
    let toml_str = match File::open(&Path::new(filename)).read_to_string() {
        Ok(str) => str,
        Err(e) => panic!("Unable to read {}: {}", filename, e)
    };
    let mut parser = toml::Parser::new(toml_str.as_slice());
    parser.parse().unwrap_or_else(|| {
        for error in parser.errors.iter() {
            let (lo_line, lo_col) = parser.to_linecol(error.lo);
            let (hi_line, hi_col) = parser.to_linecol(error.hi);
            error!("{}:{}:{}: {}:{} {}", filename, lo_line, lo_col, hi_line, hi_col, error.desc);
        }
        panic!("Failed to parse {}", filename);
    })
}

