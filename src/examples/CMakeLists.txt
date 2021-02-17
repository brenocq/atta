file(GLOB_RECURSE src_examples
	cleaningRobots2D/*.cpp
	materialsDemo/*.cpp
)

set(src_files_examples
	${src_examples}
	PARENT_SCOPE)
