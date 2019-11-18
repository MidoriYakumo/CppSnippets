import qbs

Project {
	id: project

	CppApplication {
		consoleApplication: true
		files: ["main.cpp", "callback_inserter.hpp"]
		cpp.cppFlags: [].concat(project.parent
								&& project.parent.commonCppFlags || [])
	}
}
