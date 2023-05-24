import os
import argparse
import shutil
import configparser
import glob

confPath = ".\\build.ini"

if (not os.path.isfile(confPath)):
	print("cannot find build.ini file")
	exit(1)

config = configparser.ConfigParser()
config.read(confPath)

# collect basic data from config file
compiler = config["build"]["compiler"]

defines = ""
for define in config["build"]["defines"].split(","):
	if len(define) > 0:
		defines += f"-D{define} "

rootdir = os.getcwd().replace("\\", "/")

include = os.path.join(rootdir, config["source"]["include"])
src = config["source"]["sourceRoot"]
build = config["build"]["buildFolder"]
target = config["build"]["target"]

defaultCompilerFlags = config["build"]["defaultCompilationFlags"]
linkingFlags = config["build"]["linkingFlags"]
linkedLibs = config["build"]["linkedLibs"]

ignoreCompilerFail = False
autoRunExe = True

def makeDirIfNotExisting(path: str):
	if (not os.path.isdir(path)):
		os.makedirs(path)


args = argparse.ArgumentParser()
args.add_argument("-re", "--rebuild", help="rebuilds entire project", action="store_true")
args.add_argument("--ignore-comp-fail", help="runs old target if there were compiler errors", action="store_true")
args.add_argument("-norun", help="builds and links exe, but does not automatically run it", action="store_true")

parsed = args.parse_args()

if (parsed.rebuild):
	# god, have i rebuilt everything for no reason too often
	print("Are you sure you want to rebuild everything? Y/N")

	answer = input()

	while (not (answer in ["Y", "N"])):
		answer = input()
	
	if (answer == "Y"):
		shutil.rmtree(build)

if (parsed.ignore_comp_fail):
	ignoreCompilerFail = True

if (parsed.norun):
	autoRunExe = False

# make build directory if it doesn't exist
makeDirIfNotExisting(rootdir + build)


def shouldCompile(file: str):
	# i am aware this is ugly.
	objectFile = f"{rootdir}{build}{file}.o".replace("./", "/")
	sourceFile = f"{rootdir}{src}{file}.c".replace("./", "/")

	# i'm aware that this could be 1 return, this is far more readable
	if not os.path.isfile(objectFile):
		return True
	
	if os.path.getmtime(objectFile) < os.path.getmtime(sourceFile):
		return True

	return False

# compiles source file to appropriate directory in .build, creating one if need be
def compileToObject(sourceFile: str, compileFlags: str, section: str):
	print("compiling " + sourceFile.removeprefix("/") + " from section " + section)
	startingDir = os.getcwd()
	
	os.chdir(rootdir + build)

	split = sourceFile[::-1].split("/", 1)

	# expected input is in the style "[\path]\file". we reverse it because there's no split last, 
	# split it by \ with at most 1 split and thereby get "[hatp\]". we then reverse this again
	# to get \path, and use it to generate a folder and move to it if need be
	if (len(split) == 2 and len(split[-1]) > 0):
		subdir = split[-1][::-1]
		makeDirIfNotExisting(f"{rootdir}{build}/{subdir}")
		os.chdir(subdir.removeprefix("/"))

	fixedFlags = compileFlags.replace("${defaultCompilationFlags}", defaultCompilerFlags)
	fixedSrc = src.removeprefix(".")
	command = f"{compiler} -I\"{include}\" {defines} -c \"{rootdir}{fixedSrc}{sourceFile}.c\" {fixedFlags}"

	returnCode = os.system(command)

	os.chdir(startingDir)
	
	return returnCode

print("begun compiling")

compilationFailed = False
compiledAnything = False

objectFiles = []

# compile everything
for section in config.sections():

	# use only sections starting in c file
	if section.startswith("c file"):

		# windows likes its \\ paths
		def fixPath(path):
			return path.replace("\\", "/")

		sourceFiles = list(map(fixPath, glob.glob(config[section]["path"], recursive=True)))

		# if this section has an exclude pattern, apply it
		# this is ugly, we should evaluate by section and remove selected files from list instead
		if "exclude" in config[section]:
			for excluding in config[section]["exclude"].split(","):
				subtracting = set(glob.glob(excluding, recursive=True))
				sourceFiles = list(set(sourceFiles) - subtracting)

		# actually compile the things
		for file in sourceFiles:

			if file.endswith(".c"):
				# glob returns a relative path, we wnat to strip the src so we can easily compare it
				# to a build pair
				file = file.removeprefix(src).removesuffix(".c")

				if shouldCompile(file):
					errorCode = compileToObject(file, config[section]["compilationFlags"], section)

					compiledAnything = compiledAnything or errorCode == 0
					compilationFailed = compilationFailed or errorCode != 0
				
				objectFiles.append(f"{build}{file}.o")


if compilationFailed:
	print(f"compilation failed!")

elif (compiledAnything or not os.path.isfile(f"{rootdir}/{target}")):
	# linking step
	print("building executable")
	linking = ""

	for file in objectFiles:
		linking += file + " "

	os.system(f"{compiler} -o {target} {linking} {linkedLibs} {linkingFlags}")

	print(f"executable built!")

else:
	print(f"executable up to date!")


# run the thing
if autoRunExe and (ignoreCompilerFail or not compilationFailed):
	print(f"running {target}")
	os.system(f"\"{os.path.join(rootdir, target)}\"")

elif autoRunExe and compilationFailed:
	print(f"not running {target}. use --ignore-comp-fail if you want to run anyway")
	exit(1)
