import qbs

Project {
	id: project

	CppApplication {
		consoleApplication: true
		files: ["main.cpp", "stack_storage.hpp"]
		cpp.cppFlags: [].concat(project.parent
								&& project.parent.commonCppFlags || [])
	}
}
