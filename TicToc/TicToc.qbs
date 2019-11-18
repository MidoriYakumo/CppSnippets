import qbs

Project {
	id: project

	CppApplication {
		consoleApplication: true
		files: ["main.cpp", "tic_toc.cpp", "tic_toc.hpp", "tic_toc.inl.hpp"]
		cpp.cppFlags: [].concat(project.parent
								&& project.parent.commonCppFlags || [])
		cpp.dynamicLibraries: ["glog"]

		Group {
			name: "Precompile Header"
			files: ["pch.h"]
			fileTags: ["cpp_pch_src"]
		}
	}
}
