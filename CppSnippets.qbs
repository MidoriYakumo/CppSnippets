import qbs
import qbs.File
import qbs.FileInfo

Project {
	id: rootProject
	name: "CppCollections"
	references: subProjectProbe.result

	property bool reportCompileTime: false
	property stringList commonCppFlags: {
		var ret = [].concat(["-Wall", "-Wextra", "-Wpedantic"]).concat(
					[/*"-Weffc++",*/ "-Wshadow", "-Wundef"]).concat(
					["-Winline", "-Winit-self", "-Wcast-align", "-Wcast-qual"]).concat(
					["-Wswitch-default", "-Wswitch-enum", "-Wlogical-op"]).concat(
					["-Wfloat-equal", "-Wpointer-arith"]).concat(
					["-Wredundant-decls", "-Wmissing-braces", "-Wuninitialized"]).concat(
					["-Wunreachable-code", "-Wunused-parameter", "-Wwrite-strings"]).concat(
					["-Wstrict-aliasing=2", "-Wstrict-overflow=5", "-Wformat=2"]).concat(
					["-Wno-unused-macros", "-Wno-missing-field-initializers"]).concat(
					["-Winvalid-pch"])
		if (reportCompileTime) {
			ret = ret.concat(["-H", "-ftime-report"])
		}
		return ret
	}

	Probe {
		id: subProjectProbe

		property pathList result: []

		configure: {
			console.info("Sub-projects:")

			var result_ = []
			var dirList = File.directoryEntries(rootProject.sourceDirectory,
												File.Dirs | File.NoDotAndDotDot)
			for (var idxDir in dirList) {
				var directory = FileInfo.joinPaths(rootProject.sourceDirectory,
												   dirList[idxDir])
				var fileList = File.directoryEntries(directory, File.Files)
				for (var idxFile in fileList) {
					var file = FileInfo.joinPaths(directory, fileList[idxFile])
					if (file.endsWith(".qbs")) {
						result_.push(file)
						console.info("\t" + fileList[idxFile])
					}
				}
			}

			result = result_
		}
	}
}
