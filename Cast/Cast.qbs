import qbs

Project {
	id: project

	CppApplication {
		consoleApplication: true
		files: ["main.cpp", "cast.hpp"]
		cpp.cppFlags: [].concat(project.parent
								&& project.parent.commonCppFlags || [])
	}
}
