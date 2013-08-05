flags = [
	'-Wall',
	'-Wextra',
	'-Werror',
	'-fexceptions',
	'-std=c++11',
    '-stdlib=libc++',
	'-x', 'c++'
]
def FlagsForFile(filename):
	return {
		'flags': flags,
		'do_cache': True
	}
