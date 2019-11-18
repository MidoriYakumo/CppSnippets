import qbs

Project {
	id: project

	CppApplication {
		consoleApplication: true
		files: ["main.cpp", "expire_list.hpp", "expire_list.inl.hpp"]
		cpp.cppFlags: [].concat(project.parent
								&& project.parent.commonCppFlags || [])

		Group {
			name: "Precompile Header"
			files: ["pch.h"]
			fileTags: ["cpp_pch_src"]
		}
	}
}
