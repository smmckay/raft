flags = [
	'-Wall',
	'-Wextra',
	'-Werror',
	'-fexceptions',
	'-std=c++11',
	'-x', 'c++'
]
def FlagsForFile(filename):
	return {
		'flags': flags,
		'do_cache': True
	}
