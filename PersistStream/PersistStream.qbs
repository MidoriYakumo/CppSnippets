import qbs

Project {
	id: project

	CppApplication {
		consoleApplication: true
		files: ["main.cpp", "persist_stream.cpp", "persist_stream.h", "persist_stream.inl.h"]
		cpp.cppFlags: [].concat(project.parent
								&& project.parent.commonCppFlags || [])

		Group {
			name: "Precompile Header"
			files: ["pch.h"]
			fileTags: ["cpp_pch_src"]
		}
	}
}
