import qbs

Project {
	id: project

	CppApplication {
		consoleApplication: true
		files: [
			"main.cpp",
			"frame_depth.cpp", "frame_depth.h",
			"stack_depth.cpp", "stack_depth.h", "stack_depth.inl.h"
		]
		cpp.cppFlags: [].concat(project.parent
								&& project.parent.commonCppFlags || [])

		Group {
			name: "Precompile Header"
			files: ["pch.h"]
			fileTags: ["cpp_pch_src"]
		}
	}
}

