set(Tensorflow_FOUND TRUE)

find_package(Eigen3 REQUIRED)

set(Tensorflow_INCLUDE_DIRS
	"/usr/local/include/google/tensorflow"
	"/usr/local/include/"
	${Eigen3_INCLUDE_DIRS}
)

set(Tensorflow_LIBRARIES
	# tensorflow_cc
	# tensorflow_framework

	/usr/local/lib/libtensorflow_cc.so
	/usr/local/lib/libtensorflow_framework.so

	${Eigen3_LIBRARIES}
)
