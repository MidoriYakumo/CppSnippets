import qbs

Project {
	id: project

	CppApplication {
		consoleApplication: true
		files: ["main.cpp", "defer_apply.hpp"]
		cpp.cppFlags: [].concat(project.parent
								&& project.parent.commonCppFlags || [])
	}
}
