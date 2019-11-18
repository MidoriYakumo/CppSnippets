import qbs

Project {
	id: project

	CppApplication {
		consoleApplication: true
		files: ["main.cpp", "clone_ptr.hpp", "clone_ptr.inl.hpp"]
		cpp.cppFlags: [].concat(project.parent
								&& project.parent.commonCppFlags || [])

		Group {
			name: "Precompile Header"
			files: ["pch.h"]
			fileTags: ["cpp_pch_src"]
		}
	}
}
