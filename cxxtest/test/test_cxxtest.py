#-------------------------------------------------------------------------
# CxxTest: A lightweight C++ unit testing library.
# Copyright (c) 2008 Sandia Corporation.
# This software is distributed under the LGPL License v3
# For more information, see the COPYING file in the top CxxTest directory.
# Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
# the U.S. Government retains certain rights in this software.
#-------------------------------------------------------------------------

import time
import sys
import os
import os.path
import glob
import difflib
import subprocess
import re
import string
import argparse

if sys.version_info < (2,7) :
    import unittest2 as unittest
else :
    import unittest
try :
    import ply
    ply_available = True
except :
    ply_available = False
try :
    import cxxtest
    cxxtest_available = True
    import cxxtest.cxxtestgen
except :
    cxxtest_available = False

print ("Python version: %s\n" % sys.version)

verbose         = None
currentDir      = os.path.dirname(os.path.abspath(__file__))   + os.sep
cxxtestdir      = os.path.dirname(os.path.dirname(currentDir)) + os.sep
cxxtestgenDir   = "..{0}bin{0}cxxtestgen.py".format(os.sep)
goldenFilesDir  = "{0}UT_Goldens{1}".format(currentDir, os.sep)
sourcesFilesDir = "UT_Sources{0}".format(os.sep)
sampledir       = sourcesFilesDir + os.sep + 'sample' + os.sep

compilerre = re.compile("^(?P<path>[^:]+)(?P<rest>:.*)$")
dirre      = re.compile("^([^%s]*/)*" % re.escape(os.sep))
xmlre      = re.compile("\"(?P<path>[^\"]*/[^\"]*)\"")
datere     = re.compile("date=\"[^\"]*\"")

# Headers from the cxxtest/sample directory
if sys.platform.startswith('win') :
    target_suffix     = '.exe'
    command_separator = ' && '
    cxxtestdir        = '/'.join(cxxtestdir.split('\\'))

    remove_extra_path_prefixes_on_windows = True
else :
    target_suffix     = ''
    command_separator = '; '

    remove_extra_path_prefixes_on_windows = False

# Prints message about missing PLY module
#
def printMissingPlyMessage():
  print ("Python Ply (Python Lex-Yacc) module is required for FOG parser!\nInstall PLY with command: 'python -m pip install ply'")
#
#  End of function 'printMissingPlyMessage'
#---------------------------------------------------------------------------


def find(filename, executable = False, isfile = True,  validate = None) :
    #
    # Use the PATH environment if it is defined and not empty
    #
    if "PATH" in os.environ and os.environ["PATH"] != "" :
        search_path = os.environ['PATH'].split(os.pathsep)
    else :
        search_path = os.defpath.split(os.pathsep)
    for path in search_path :
            test_fname = os.path.join(path, filename)
            if os.path.exists(test_fname) \
                   and (not isfile or os.path.isfile(test_fname)) \
                   and (not executable or os.access(test_fname, os.X_OK)) :
                return os.path.abspath(test_fname)
    return None

def join_commands(command_one, command_two) :
    return command_separator.join([command_one, command_two])

_available = {}
def available(testClass, compiler, exe_option, linkOptions = '') :
    """
    " Check for operational compiler and options.
    " Returns true if tests can be run using specified compiler
    """
    if (compiler, exe_option) in _available :
        return _available[compiler, exe_option]

    exePathSwitch = testClass.getExePathSwitch('anything' + target_suffix)
    sourceFile    = sourcesFilesDir + 'anything.cpp'
    cmd = join_commands("cd %s" % currentDir,
                        "%s %s %s %s > %s 2>&1" % (compiler, exePathSwitch, sourceFile, linkOptions, currentDir + 'anything.log'))

    print("\n\nTesting for compiler '{0}'".format(compiler))
    print("Command: " + cmd)
    status = subprocess.call(cmd, shell = True)
    executable = currentDir + 'anything' + target_suffix
    flag = status == 0 and os.path.exists(executable)
    os.remove(currentDir + 'anything.log')
    if os.path.exists(executable) :
        os.remove(executable)
    print("Status: " + str(flag))
    _available[compiler, exe_option] = flag
    if flag :
        print ("Will use compiler: {0} with options: {1}".format(compiler, exe_option))

    return flag

def remove_absdir(filename) :
    INPUT = open(filename, 'r')
    lines = [line.strip() for line in INPUT]
    INPUT.close()
    OUTPUT = open(filename, 'w')
    for line in lines :
        # remove basedir at front of line
        match = compilerre.match(line) # see if we can remove the basedir
        if match :
            parts = match.groupdict()
            line = dirre.sub("", parts['path']) + parts['rest']
        OUTPUT.write(line + '\n')
    OUTPUT.close()

def normalize_line_for_diff(line) :
    # add spaces around {}<>()
    line = re.sub("[{}<>()]", r" \0 ", line)

    # beginnig and ending whitespace
    line = line.strip()

    # remove all whitespace
    # and leave a single space
    line = ' '.join(line.split())

    # remove spaces around "="
    line = re.sub(" ?= ?", "=", line)

    # remove all absolute path prefixes
    line = ''.join(line.split(cxxtestdir))

    if remove_extra_path_prefixes_on_windows :
        # Take care of inconsistent path prefixes like
        # "e:\path\to\cxxtest\test", "E:/path/to/cxxtest/test" etc
        # in output.
        line = ''.join(line.split(os.path.normcase(cxxtestdir)))
        line = ''.join(line.split(os.path.normpath(cxxtestdir)))
        # And some extra relative paths left behind
        line = re.sub(r'^.*[\\/]([^\\/]+\.(h|cpp))', r'\1', line)

    # for xml, remove prefixes from everything that looks like a
    # file path inside ""
    line = xmlre.sub(
            lambda match : '"' + re.sub("^[^/]+/", "", match.group(1)) + '"',
            line
            )
    # Remove date info
    line = datere.sub( lambda match : 'date=""', line)
    return line

def make_diff_readable(diff) :
    i = 0
    while i + 1 < len(diff) :
        if diff[i][0] == '-' and diff[i + 1][0] == '+' :
            l1 = diff[i]
            l2 = diff[i + 1]
            for j in range(1, min([len(l1), len(l2)])) :
                if l1[j] != l2[j] :
                    if j > 4 :
                        j = j - 2;
                        l1 = l1[j : ]
                        l2 = l2[j : ]
                        diff[i] = '-(...)' + l1
                        diff[i + 1] = '+(...)' + l2
                    break
        i += 1

def file_diff(filename1, filename2, filtered_reader) :
    remove_absdir(filename1)
    remove_absdir(filename2)
    #
    INPUT = open(filename1, 'r')
    lines1 = list(filtered_reader(INPUT))
    INPUT.close()
    #
    INPUT = open(filename2, 'r')
    lines2 = list(filtered_reader(INPUT))
    INPUT.close()
    #
    diff = list(difflib.unified_diff(lines2, lines1,
        fromfile = filename2, tofile = filename1))
    if diff :
        make_diff_readable(diff)
        raise Exception("ERROR: \n\n%s\n\n%s\n\n" % (lines1, lines2))
    diff = '\n'.join(diff)
    return diff


class BaseTestCase(object) :
    testId         = 1;   # This is used to easily identify progression
    fog            = ''
    valgrind       = ''
    cxxtest_import = False
    testOutputsDir = ''

    def setUp(self) :
        sys.stderr.write("%2d ==> %s:%s: " % (BaseTestCase.testId, self.__class__.__name__, self.shortDescription()))
        BaseTestCase.testId += 1

        self.passed       = False
        self.prefix       = ''
        self.py_out       = ''
        self.py_cpp       = ''
        self.testExecLog  = ''
        self.px_out       = ''
        self.build_log    = ''
        self.build_target = ''

    def tearDown(self) :
        if not self.passed :
            return
        files = []
        if os.path.exists(self.py_out) :
            files.append(self.py_out)
        if os.path.exists(self.py_cpp) and not 'CXXTEST_GCOV_FLAGS' in os.environ :
            files.append(self.py_cpp)
        if os.path.exists(self.testExecLog) :
            files.append(self.testExecLog)
        if os.path.exists(self.px_out) :
            files.append(self.px_out)
        if os.path.exists(self.build_log) :
            files.append(self.build_log)
        if os.path.exists(self.build_target) and not 'CXXTEST_GCOV_FLAGS' in os.environ :
            files.append(self.build_target)

        filesNotRemoved = False
        for file in files :
            try :
                os.remove(file)
            except :
                fileNotRemoved = True

        if filesNotRemoved:
          time.sleep(1)
          for file in files :
            try :
              if os.path.exists(file):
                os.remove(file)
            except :
              print("Error removing file '%s'" % file)


    # This is a "generator" that just reads a file and normalizes the lines
    def file_filter(self, file) :
        for line in file :
            yield normalize_line_for_diff(line)


    def check_if_supported(self, filename, msg) :
        self.init(filename)
        cppSources = [filename]

        self.py_cpp = ""    # This is to disable usage of normally generated .cpp
        compilerCommand = self.getCompilerCommand(cppSources)
        cmd = join_commands("cd %s" % currentDir, compilerCommand)
        status = subprocess.call(cmd, shell = True)
        if status != 0 or not os.path.exists(self.build_target) :
            os.remove(self.build_log)
            self.skipTest(msg)

        os.remove(self.build_log)
        os.remove(self.build_target)

    def initDirectory (self) :
        self.testContext            = self.__class__.__name__
        self.testOutputsRelativeDir = "UT_{0}{1}".format(self.testContext, os.sep)
        self.testOutputsDir         = "{0}{1}".format(currentDir, self.testOutputsRelativeDir)

        if not os.path.exists(self.testOutputsDir) :
            print ("Will create directory: '{0}' ".format(self.testOutputsDir))
            os.mkdir(self.testOutputsDir)


    def init(self, prefix) :
        self.initDirectory()
        self.prefix         = self.testContext    + prefix
        self.py_out         = self.testOutputsDir + prefix + '_py.out'
        self.py_cpp         = self.testOutputsDir + prefix + '_py.cpp'
        self.testExecLog    = self.testOutputsDir + prefix + '.out'
        self.px_out         = self.testOutputsDir + prefix + '_px.out'
        self.build_log      = self.testOutputsDir + prefix + '_build.log'
        self.build_target   = self.testOutputsDir + prefix + '_px' + target_suffix

    def getExePathSwitch(self, exeName) :
        return "{0} {1}".format(self.exe_option, exeName)


    def getIncludes(self) :
        includes = " {0}{1} {0}{1}..\.".format(self.include_option, currentDir)
        return includes


    def getAllSourcesAsString(self, cppSources = []) :
        allSourcesAsString = self.py_cpp
        for cppSource in cppSources :
            if cppSource :
                allSourcesAsString += " {0}{1}".format(sourcesFilesDir, cppSource)

        if verbose :
            print ("allSourcesAsString: {0}".format(allSourcesAsString))
        return allSourcesAsString



    def getCompilerCommand(self, cppSources, haveExceptionHandling = False, compilerOptionalSwitches = '') :
        compilerOptionsAndSwitches = self.compiler;
        if haveExceptionHandling :
            compilerOptionsAndSwitches += " " + self.DefinePrefix + "_CXXTEST_HAVE_EH"
            if verbose == True : print ("compilerOptionsAndSwitches: {0}".format(compilerOptionsAndSwitches))

        exePathSwitch      = self.getExePathSwitch(self.build_target)
        includes           = self.getIncludes()
        allSourcesAsString = self.getAllSourcesAsString(cppSources)

        compilerCommand = "%s %s %s %s  %s > %s 2>&1" % (compilerOptionsAndSwitches, exePathSwitch, compilerOptionalSwitches, includes, allSourcesAsString, self.build_log)
        return compilerCommand


    def check_root(self, prefix = '', output = None) :
        self.init(prefix)
        args = "--have-eh --abort-on-fail --root --error-printer"
        if self.cxxtest_import :
            os.chdir(currentDir)
            cxxtest.cxxtestgen.main(['cxxtestgen', self.fog, '-o', self.py_cpp] + re.split('[ ]+', args), True)
        else :
            cxxtestgenCmd = "%s %s%s %s -o %s %s > %s 2>&1" % (sys.executable, currentDir, cxxtestgenDir, self.fog, self.py_cpp, args, self.py_out)
            cmd = join_commands("cd %s" % currentDir, cxxtestgenCmd)
            status = subprocess.call(cmd, shell = True)
            self.assertEqual(status, 0, 'Error executing cxxtestgen -1-')
        #
        files = [self.py_cpp]
        for i in [1, 2] :
            args = "--have-eh --abort-on-fail --part %sPart%s.h" % (sourcesFilesDir, str(i))
            file = currentDir + prefix + '_py%s.cpp' % str(i)
            files.append(file)
            if self.cxxtest_import :
                os.chdir(currentDir)
                cxxtest.cxxtestgen.main(['cxxtestgen', self.fog, '-o', file] + re.split('[ ]+', args), True)
            else :
                cxxtestgenCmd = "%s %s%s %s -o %s %s > %s 2>&1" % (sys.executable, currentDir, cxxtestgenDir, self.fog, file, args, self.py_out)
                cmd = join_commands("cd %s" % currentDir, cxxtestgenCmd)
                status = subprocess.call(cmd, shell = True)
                self.assertEqual(status, 0, 'Error executing cxxtestgen -2-')
        #
        compilerBase    = self.compiler + " " + self.DefinePrefix + "_CXXTEST_HAVE_EH"
        compilerCommand = "%s %s %s %s. %s%s../ %s > %s 2>&1" % (compilerBase, self.exe_option, self.build_target, self.include_option, self.include_option, currentDir, ' '.join(files), self.build_log)
        cmd = join_commands("cd %s" % currentDir, compilerCommand)

        status = subprocess.call(cmd, shell = True)
        for file in files :
            if os.path.exists(file) :
                os.remove(file)
        self.assertEqual(status, 0, 'Error executing command: ' + cmd)
        #
        cmd = join_commands("cd %s" % currentDir,
                            "%s %s -v > %s 2>&1" % (self.valgrind, self.build_target, self.testExecLog))
        status = subprocess.call(cmd, shell = True)
        OUTPUT = open(self.testExecLog, 'a')
        OUTPUT.write('Error level = ' + str(status) + '\n')
        OUTPUT.close()

        failureMessage = None
        try :
            diffstr = file_diff(self.testExecLog, goldenFilesDir + output, self.file_filter)
        except Exception as excp :
            failureMessage = "Failure to compare files: \n{0} and '{1}'\n{2}".format(self.testExecLog, output, excp)

        if failureMessage is not None :
            self.fail(failureMessage)
        if not diffstr == '' :
            self.fail("Unexpected differences in output while comparing files: \n{0} and '{1}:\n{2}".format(self.testExecLog, output, diffstr))
        if self.valgrind != '' :
            self.parse_valgrind(self.testExecLog)
        #
        self.passed = True

    def compile(self,
                prefix                   = '',
                args                     = '',
                hppSources               = None,
                cppSources               = None,
                compilerOptionalSwitches = None,
                haveExceptionHandling    = True,
                output                   = None,
                main                     = False,
                failGen                  = False,
                run                      = None,
                runnerOptions            = None,
                logfile                  = None,
                failBuild                = False) :
        """Runs cxxtestgen and compiles the code that is generated along with optional cpp sources"""
        self.init(prefix)

        # ---------------- Creating runner (using cxxtestgen)
        #
        allSourcesRelativePath = [] if (self.cxxtest_import) else (' ')
        if hppSources is not None :
            for hppSource in hppSources :
                if hppSource is not None :
                    allSourcesRelativePath += " {0}{1}".format(sourcesFilesDir, hppSource)

        if self.cxxtest_import :
            try :
                if verbose :   print ("Using cxxtest_import")
                os.chdir(currentDir)
                status = cxxtest.cxxtestgen.main(['cxxtestgen', self.fog, '-o', self.py_cpp] + re.split('[ ]+', args) + allSourcesRelativePath, True)
            except :
                status = 1
        else :
            cmd = join_commands("cd %s" % currentDir,
                            "%s %s%s %s -o %s %s > %s 2>&1" % (sys.executable, currentDir, cxxtestgenDir, self.fog, self.py_cpp, args + allSourcesRelativePath, self.py_out))
            if verbose : print ("\nCxxtestgen cmd: {0}".format(cmd))
            status = subprocess.call(cmd, shell = True)

        # ---------------- Check if the unit tests main generation was expected to fail
        #
        if failGen :
            if status == 0 :
                self.fail('Expected cxxtestgen to fail.')
            else :
                self.passed = True
                return

        if not self.cxxtest_import :
            self.assertEqual(status, 0, 'Error executing command: ' + cmd)

        # Compiling source files
        if cppSources is None : cppSources = []
        if main : cppSources += ['main.cpp']

        if compilerOptionalSwitches is None : compilerOptionalSwitches = ""

        compilerCommand = self.getCompilerCommand(cppSources, haveExceptionHandling, compilerOptionalSwitches)
        cmd = join_commands("cd %s" % currentDir, compilerCommand)
        if verbose: print ("\nCompiler command: %s" % compilerCommand)

        cppSources = []
        status = subprocess.call(cmd, shell = True)
        if failBuild :
            if status == 0 :
                self.fail('Expected compiler to fail.')
            else :
                self.passed = True
                os.remove(self.build_log)
                return
        else :
            self.assertEqual(status, 0, 'Error executing command: ' + cmd)
        #
        if output is not None :
            if run is None :
                options = "" if runnerOptions is None else runnerOptions

                cmd = join_commands("cd %s" % currentDir,
                                    "%s %s -v %s > %s 2>&1" % (self.valgrind, self.build_target, options, self.testExecLog))
            else :
                cmd = run % (self.valgrind, self.build_target, self.testExecLog)

            status = subprocess.call(cmd, shell = True)
            OUTPUT = open(self.testExecLog, 'a')
            OUTPUT.write('Error level = ' + str(status) + '\n')
            OUTPUT.close()

            if status >= 255 :
                self.fail("Got error {1} when running cmd: \n{0}".format(cmd, status))

            referenceFilePath = self.testExecLog if logfile is None else logfile
            referenceFileName = self.testExecLog if logfile is None else logfile
            if not os.path.exists(referenceFilePath) :
                self.fail("There is no log file from running: {0}".format(cmd))

            failureMessage = None
            try :
                diffstr = file_diff(referenceFilePath, goldenFilesDir + output, self.file_filter)
            except Exception as excp :
                failureMessage = "Failure to compare files: \n{0} and '{1}'\n{2}".format(referenceFileName, output, excp)

            if failureMessage is not None :
                failureMessage = "After running cmd: {0}\n".format(cmd) + failureMessage
                self.fail(failureMessage)
            if not diffstr == '' :
                self.fail("Unexpected differences while comparing files: \n{0} and, golden, '{1}:\n{2}".format(referenceFileName, output, diffstr))
            if self.valgrind != '' :
                self.parse_valgrind(self.testExecLog)

            if not logfile is None :
                os.remove(currentDir + logfile)
        #
        if compilerOptionalSwitches == '' and output is None and os.path.exists(self.py_cpp) :
            self.fail("Output cpp file %s should not have been generated." % self.py_cpp)
        #
        self.passed = True



    #
    # Tests for cxxtestgen
    #
    samples = []
    for file in glob.iglob(sampledir + '*.h') :
        samples += ['sample' + os.sep + os.path.basename(file)]
    samples = sorted(samples)

    def test_root_or_part(self) :
        """Root/Part"""
        self.check_root(prefix = 'root_or_part', output = "parts.out")

    def test_root_plus_part(self) :
        """Root + Part"""
        self.compile(prefix = 'root_plus_part', args = "--error-printer --root --part", hppSources = self.samples, output = "samples_commons.out")

    def test_wildcard(self) :
        """Wildcard input"""
        self.compile(prefix = 'wildcard', args = sampledir + '*.h', main = True, output = "wildcard.out")

    def test_stdio_printer(self) :
        """Stdio printer"""
        self.compile(prefix = 'stdio_printer', args = "--runner=StdioPrinter ", hppSources = self.samples, output = "samples_commons.out")

    def test_paren_printer(self) :
        """Paren printer"""
        self.compile(prefix = 'paren_printer', args = "--runner=ParenPrinter ", hppSources = self.samples, output = "paren_printer.out")

    def test_yn_runner(self) :
        """Yes/No runner"""
        self.compile(prefix = 'yes_no_runner', args = "--runner=YesNoRunner ", hppSources = self.samples, output = "yes_no_runner.out")

    def test_no_static_init(self) :
        """No static init"""
        self.compile(prefix = 'no_static_init', args = "--error-printer --no-static-init --have-std", hppSources = self.samples, output = "samples_commons.out")

    def test_samples_file(self) :
        """Samples file"""
        # Create a file with the list of sample files
        OUTPUT = open(currentDir + 'Samples.txt', 'w')
        for line in sorted(glob.glob(sampledir + '*.h')) :
            OUTPUT.write(line + '\n')
        OUTPUT.close()
        self.compile(prefix = 'samples_file', args = "--error-printer --headers Samples.txt", output = "samples_commons.out")
        os.remove(currentDir + 'Samples.txt')

    def test_have_std(self) :
        """Have Std"""
        self.compile(prefix = 'have_std', args = "--runner=StdioPrinter --have-std", hppSources = ['HaveStd.h'], output = "have_std.out")

    def test_comments(self) :
        """Comments"""
        self.compile(prefix = 'comments', args = "--error-printer", hppSources = ['Comments.h'], output = "comments.out")

    def test_longlong(self) :
        """Long long"""
        self.check_if_supported('longlong.cpp', "Long long is not supported by this compiler")
        self.compile(prefix = 'longlong', args = "--error-printer --longlong=\"long long\"", hppSources = ['LongLong.h'], output = "longlong.out")

    def test_int64(self) :
        """Int64"""
        self.check_if_supported('int64.cpp', "64-bit integers are not supported by this compiler")
        self.compile(prefix = 'int64', args = "--error-printer --longlong=__int64", hppSources = ['Int64.h'], output = "int64.out")

    def test_include(self) :
        """Include"""
        includes  = "--include=" + sourcesFilesDir + 'Traits_Void.h '
        includes += "--include=" + sourcesFilesDir + 'Traits_Long.h '
        self.compile(prefix = 'include', args = includes + "--error-printer", hppSources = ['IncludeTest.h'], output = "include.out")

    #
    # Template file tests
    #

    def test_preamble(self) :
        """Preamble"""
        self.compile(prefix = 'preamble', args = "--template=preamble.tpl ", hppSources = self.samples, output = "preamble.out")

    def test_activate_all(self) :
        """Activate all"""
        self.compile(prefix = 'activate_all', args = "--template=activate.tpl ", hppSources = self.samples, output = "samples_commons.out")

    def test_only_suite(self) :
        """Only Suite"""
        self.compile(prefix = 'only_suite', args = "--template=only.tpl", hppSources = self.samples, run = "%s %s SimpleTest > %s 2>&1", output = "only_suite.out")

    def test_only_test(self) :
        """Only Test"""
        self.compile(prefix = 'only_test', args = "--template=only.tpl", hppSources = self.samples, run = "%s %s SimpleTest testAddition > %s 2>&1", output = "only_test.out")

    def test_have_std_tpl(self) :
        """Have Std - Template"""
        self.compile(prefix = 'have_std_tpl', args = "--template=HaveStd.tpl", hppSources = ['HaveStd.h'], output = "have_std_tpl.out")

    def test_exceptions_tpl(self) :
        """Exceptions - Template"""
        self.compile(prefix = 'have_eh_tpl', args = "--template=HaveEH.tpl ", hppSources = self.ehNormals, output = "have_eh_tpl.out")

    #
    # Test cases which do not require exception handling
    #

    def test_no_errors(self) :
        """No errors"""
        self.compile(prefix = 'no_errors', args = "--error-printer", hppSources = ['GoodSuite.h'], output = "no_errors.out")

    def test_infinite_values(self) :
        """Infinite values"""
        self.compile(prefix = 'infinite_values', args = "--error-printer --have-std", hppSources = ['TestNonFinite.h'], output = "infinite_values.out")

    def test_max_dump_size(self) :
        """Max dump size"""
        self.compile(prefix = 'max_dump_size', args = "--error-printer --include=" + sourcesFilesDir + 'MaxDump.h', hppSources = ['DynamicMax.h', 'SameData.h'], output = 'max_dump_size.out')

    def test_wide_char(self) :
        """Wide char"""
        self.check_if_supported('wchar.cpp', "The file wchar.cpp is not supported.")
        self.compile(prefix = 'wide_char', args = "--error-printer", hppSources = ['WideCharTest.h'], output = "wide_char.out")

    #def test_factor(self):
        #"""Factor"""
        #self.compile(prefix='factor', args="--error-printer --factor Factor.h", output="factor.out")

    def test_string_traits(self) :
        """String traits"""
        self.compile(prefix = 'traits_strings', args = "--error-printer --have-std", hppSources = ['Traits_Strings.h'], output = 'traits_strings.out')

    def test_integral_traits(self) :
        """Integral traits"""
        self.compile(prefix = 'traits_integrals', args = "--error-printer --have-std", hppSources = ['Traits_Integrals.h'], output = 'traits_integrals.out')

    def test_user_traits(self) :
        """User traits"""
        self.compile(prefix = 'traits_user', args = "--template=UserTraits.tpl", hppSources = ['Traits_User.h'], output = 'traits_user.out')

    def test_stl_traits(self) :
        """STL Traits"""
        self.check_if_supported('stpltpl.cpp', "The file stpltpl.cpp is not supported.")
        self.compile(prefix = 'traits_stl', args = "--error-printer", hppSources = ['Traits_Stl.h'], output = "traits_stl.out")

    normals = ["LessThanEquals.h", "Relation.h", "Traits_Default.h", "DoubleCall.h", "SameData.h", "SameFiles.h", "Tsm.h", "Traits_Strings.h", "Traits_Integrals.h", "MockTest.h", "SameZero.h"]

    def test_normal_behavior_xunit(self) :
        """Normal Behavior with XUnit Output"""
        self.initDirectory()    # This must be done there because the class "init" method is called only by "compile" method
        logFile = self.testOutputsRelativeDir + 'normal_behavior_xunit.xml'
        self.compile(prefix = 'normal_behavior_xunit', args = "--xunit-printer --xunit-file=" + logFile, logfile = logFile, hppSources = self.normals, output = "normal_behavior_xunit.xml")

    def test_normal_behavior(self) :
        """Normal Behavior"""
        self.compile(prefix = 'normal_behavior', args = "--error-printer ", hppSources = self.normals, output = "normal_behavior.out")

    def test_normal_plus_abort(self) :
        """Normal + Abort"""
        self.compile(prefix = 'normal_plus_abort', args = "--error-printer --have-eh --abort-on-fail", hppSources = self.normals, output = "normal_plus_abort.out")


    def test_normal_behavior_world(self) :
        """Normal Behavior with World"""
        self.compile(prefix = 'normal_behavior_world', args = "--error-printer --world=myworld", hppSources = self.normals, output = "normal_behavior_world.out")

    #
    # Test cases which do require exception handling
    #
    def test_throw_wo_std(self) :
        """Throw w/o Std"""
        self.compile(prefix = 'throw_no_std', args = "--template=ThrowNoStd.tpl", hppSources = ['ThrowNoStd.h'], output = 'throw_no_std.out')

    ehNormals = ["Exceptions.h", "DynamicAbort.h"]

    def test_exceptions(self) :
        """Exceptions"""
        self.compile(prefix = 'eh_normals', args = "--error-printer --have-eh ", hppSources = self.ehNormals, output = "eh_normals.out")

    def test_exceptions_plus_abort(self) :
        """Exceptions plus abort"""
        self.compile(prefix = 'eh_plus_abort_on_fail', args = "--error-printer --abort-on-fail --have-eh", hppSources = ['DynamicAbort.h', 'DeepAbort.h', 'ThrowsAssert.h'], output = "eh_plus_abort_on_fail.out")

    def test_default_abort(self) :
        """Default abort"""
        self.compile(prefix = 'default_abort', args = "--error-printer --include=" + sourcesFilesDir + "DefaultAbort.h", hppSources = self.ehNormals + ['DeepAbort.h', 'ThrowsAssert.h'], output = "default_abort.out")

    def test_default_no_abort(self) :
        """Default no abort"""
        self.compile(prefix = 'default_no_abort', args = "--error-printer ", hppSources = self.ehNormals + ['DeepAbort.h', 'ThrowsAssert.h'], output = "default_no_abort.out")

    #
    # Global Fixtures
    #

    def test_global_fixtures(self) :
        """Global fixtures"""
        self.compile(prefix = 'global_fixtures', args = "--error-printer", hppSources = ['GlobalFixtures.h', 'WorldFixtures.h'], output = "global_fixtures.out")

    def test_gf_suw_fails(self) :
        """GF:SUW fails"""
        self.compile(prefix = 'gf_suw_fails', args = "--error-printer", hppSources = ['SetUpWorldFails.h'], output = "gf_suw_fails.out")

    def test_gf_suw_error(self) :
        """GF:SUW error"""
        self.compile(prefix = 'gf_suw_error', args = "--error-printer", hppSources = ['SetUpWorldError.h'], output = "gf_suw_error.out")

    def test_gf_suw_throws(self) :
        """GF:SUW throws"""
        self.compile(prefix = 'gf_suw_throws', args = "--error-printer", hppSources = ['SetUpWorldThrows.h'], output = "gf_suw_throws.out")

    def test_gf_su_fails(self) :
        """GF:SU fails"""
        self.compile(prefix = 'gf_su_fails', args = "--error-printer", hppSources = ['GfSetUpFails.h'], output = "gf_su_fails.out")

    def test_gf_su_throws(self) :
        """GF:SU throws"""
        self.compile(prefix = 'gf_su_throws', args = "--error-printer", hppSources = ['GfSetUpThrows.h'], output = "gf_su_throws.out")

    def test_gf_td_fails(self) :
        """GF:TD fails"""
        self.compile(prefix = 'gf_td_fails', args = "--error-printer", hppSources = ['GfTearDownFails.h'], output = "gf_td_fails.out")

    def test_gf_td_throws(self) :
        """GF:TD throws"""
        self.compile(prefix = 'gf_td_throws', args = "--error-printer", hppSources = ['GfTearDownThrows.h'], output = "gf_td_throws.out")

    def test_gf_tdw_fails(self) :
        """GF:TDW fails"""
        self.compile(prefix = 'gf_tdw_fails', args = "--error-printer", hppSources = ['TearDownWorldFails.h'], output = "gf_tdw_fails.out")

    def test_gf_tdw_throws(self) :
        """GF:TDW throws"""
        self.compile(prefix = 'gf_tdw_throws', args = "--error-printer", hppSources = ['TearDownWorldThrows.h'], output = "gf_tdw_throws.out")

    #
    # GUI
    #
    def test_gui(self) :
        """GUI"""
        self.compile(prefix = 'gui', args = '--gui=DummyGui', hppSources = ['GreenYellowRed.h'], output = "gui.out")

    def test_gui_runner(self) :
        """GUI + runner"""
        self.compile(prefix = 'gui_paren_printer', args = "--gui=DummyGui --runner=ParenPrinter", hppSources = ['GreenYellowRed.h'], output = "gui_paren_printer.out")

    def test_qt_gui(self) :
        """QT GUI"""
        self.compile(prefix = 'qt_gui', args = "--gui=QtGui", hppSources = ['GoodSuite.h'], compilerOptionalSwitches = self.qtFlags)

    def test_win32_gui(self) :
        """Win32 GUI"""
        self.compile(prefix = 'win32_gui', args = "--gui=Win32Gui", hppSources = ['GoodSuite.h'], compilerOptionalSwitches = self.w32Flags)

    def test_win32_unicode(self) :
        """Win32 Unicode"""
        self.compile(prefix = 'win32_unicode', args = "--gui=Win32Gui", hppSources = ['GoodSuite.h'], compilerOptionalSwitches = self.w32Flags + ' -DUNICODE')

    def test_x11_gui(self) :
        """X11 GUI"""
        self.check_if_supported('wchar.cpp', "Cannot compile wchar.cpp")
        self.compile(prefix = 'x11_gui', args = "--gui=X11Gui", hppSources = ['GoodSuite.h'], compilerOptionalSwitches = self.x11Flags)


    #
    # Tests for when the compiler doesn't support exceptions
    #

    def test_no_exceptions(self) :
        """No exceptions"""
        if self.no_eh_option is None :
            self.skipTest("This compiler does not have an exception handling option")
        self.compile(prefix = 'no_exceptions', haveExceptionHandling = False, args = '--runner=StdioPrinter', hppSources = ['NoEh.h'], output = "no_exceptions.out", compilerOptionalSwitches = self.no_eh_option)

    def test_force_no_eh(self) :
        """Force no EH"""
        if self.no_eh_option is None :
            self.skipTest("This compiler does not have an exception handling option")
        self.compile(prefix = "force_no_eh", haveExceptionHandling = False, args = "--runner=StdioPrinter --no-eh", hppSources = ['ForceNoEh.h'], output = "force_no_eh.out", compilerOptionalSwitches = self.no_eh_option)

    #
    # Invalid input to cxxtestgen
    #

    def test_no_tests(self) :
        """No tests"""
        self.compile(prefix = 'no_tests', args = 'EmptySuite.h', failGen = True)

    def test_missing_input(self) :
        """Missing input"""
        self.compile(prefix = 'missing_input', args = '--template=NoSuchFile.h', failGen = True)

    def test_missing_template(self) :
        """Missing template"""
        self.compile(prefix = 'missing_template', args = '--template=NoSuchFile.h', hppSources = self.samples, failGen = True)

    def test_inheritance(self) :
        """Test relying on inheritance"""
        self.compile(prefix = 'inheritance_old', args = '--error-printer InheritedTest.h', output = 'inheritance_old.out')

    #
    # Tests that illustrate differences between the different C++ parsers
    #

    def test_namespace1(self) :
        """Nested namespace declarations"""
        if self.fog == '' :
            self.compile(prefix = 'namespace_1', hppSources = ['Namespace1.h'], main = True, failBuild = True)
        else :
            self.compile(prefix = 'namespace_1', hppSources = ['Namespace1.h'], main = True, output = "namespace_1.out")

    def test_namespace2(self) :
        """Explicit namespace declarations"""
        self.compile(prefix = 'namespace_2', hppSources = ['Namespace2.h'], main = True, output = "namespace_2.out")

    def test_inheritance(self) :
        """Test relying on inheritance"""
        if self.fog == '' :
            self.compile(prefix = 'inheritance', args = '--error-printer', hppSources = ['InheritedTest.h'], failGen = True)
        else :
            self.compile(prefix = 'inheritance', args = '--error-printer', hppSources = ['InheritedTest.h'], output = 'inheritance.out')

    def test_simple_inheritance(self) :
        """Test relying on simple inheritance"""
        self.compile(prefix = 'simple_inheritance', args = '--error-printer', hppSources = ['SimpleInheritedTest.h'], output = 'simple_inheritance.out')

    def test_simple_inheritance2(self) :
        """Test relying on simple inheritance (2)"""
        if self.fog == '' :
            self.compile(prefix = 'simple_inheritance_2', args = '--error-printer', hppSources = ['SimpleInheritedTest2.h'], failGen = True)
        else :
            self.compile(prefix = 'simple_inheritance_2', args = '--error-printer', hppSources = ['SimpleInheritedTest2.h'], output = 'simple_inheritance_2.out')

    def test_comments2(self) :
        """Comments2"""
        if self.fog == '' :
            self.compile(prefix = 'comments_2', args = "--error-printer", hppSources = ['Comments2.h'], failBuild = True)
        else :
            self.compile(prefix = 'comments_2', args = "--error-printer", hppSources = ['Comments2.h'], output = 'comments_2.out')

    def test_cpp_template1(self) :
        """C++ Templates"""
        if self.fog == '' :
            self.compile(prefix = 'cpp_template', args = "--error-printer", hppSources = ['CppTemplateTest.h'], failGen = True)
        else :
            self.compile(prefix = 'cpp_template', args = "--error-printer", hppSources = ['CppTemplateTest.h'], output = 'cpp_template.out')

    def test_bad1(self) :
        """BadTest1"""
        if self.fog == '' :
            self.compile(prefix = 'bad_Test', args = "--error-printer", hppSources = ['BadTest.h'], failGen = True)
        else :
            self.compile(prefix = 'bad_Test', args = "--error-printer", hppSources = ['BadTest.h'], output = 'bad_test.out')

    #
    # JFC added tests (for modified/added assertions)
    #
    def test_not_assert_macros(self) :
        """Tests 'not assertion' traits"""
        self.compile(prefix = 'test_not_assert_macros', args = '--error-printer --world=UT_NotAssertMacros', hppSources = ['UT_NotAssertMacros.h'], cppSources = ['UT_NotAssertMacros.cpp'], output = 'test_not_assert_macros.out')

    def test_value_traits(self) :
        """Tests values traits"""
        self.compile(prefix = 'test_value_traits', args = '--error-printer --world=UT_CxxtestValueTraits  --longlong=\"long long\"', hppSources = ['UT_ValueTraits.h'], cppSources = ['UT_ValueTraits.cpp'], output = 'test_value_traits.out')

    def test_asserts_successes(self) :
        """test_asserts_successes"""
        self.compile(prefix = 'test_asserts_successes', args = '--error-printer --world=UT_Asserts_Successes  --longlong=\"long long\"', hppSources = ['UT_Asserts_Successes.h'], cppSources = ['UT_Asserts_Successes.cpp'], output = 'test_asserts_successes.out')

    def test_asserts_successes_details (self) :
        """test_asserts_successes_details"""
        self.compile(prefix = 'test_asserts_successes_details', runnerOptions = '--display_success', args = '--error-printer --world=UT_Asserts_Successes  --longlong=\"long long\"', hppSources = ['UT_Asserts_Successes.h'], cppSources = ['UT_Asserts_Successes.cpp'], output = 'test_asserts_successes_details.out')

    def test_asserts_failures(self) :
        """Tests assertions failures"""
        self.compile(prefix = 'test_asserts_failures', args = '--error-printer --world=UT_Asserts_Failures  --longlong=\"long long\"', hppSources = ['UT_Asserts_Failures.h'], cppSources = ['UT_Asserts_Failures.cpp'], output = 'test_asserts_failures.out')

    def test_abort_test_on_fail(self) :
        """Tests 'abort test on failure' for most assertions"""
        self.compile(prefix = 'test_abort_test_on_fail', args = '--error-printer --world=UT_AbortTestOnFailure', hppSources = ['UT_AbortTestOnFail.h'], cppSources = ['UT_AbortTestOnFail.cpp'], output = 'test_abort_test_on_fail.out')

    def test_unexpected_exceptions(self) :
        """Tests 'abort test on failure' for most assertions"""
        self.compile(prefix = 'test_unexpected_exceptions', args = '--error-printer --world=UT_UnexpectedExceptions', hppSources = ['UT_UnexpectedExceptions.h'], cppSources = ['UT_UnexpectedExceptions.cpp'], output = 'test_unexpected_exceptions.out')

    def test_expected_exceptions(self) :
        """Tests 'expecting exception' assertions"""
        self.compile(prefix     = 'test_expected_exceptions',
                     args       = '--error-printer  --have-std --world=UT_ExpectedExceptions',
                     compilerOptionalSwitches = "-DCXXTEST_HAVE_STD",
                     hppSources = ['UT_ExpectedExceptions.h'],
                     cppSources = ['UT_ExpectedExceptions.cpp'],
                     output     = 'test_expected_exceptions.out')

    def test_predicates(self) :
        """Tests 'predicates and relations assertions with lambda expression' """
        self.compile(prefix = 'test_predicates', args = '--error-printer --world=UT_Predicates', hppSources = ['UT_Predicates.h'], cppSources = ['UT_Predicates.cpp'], output = 'test_predicates.out', compilerOptionalSwitches = self.std14Flags)

    def test_pointer_asserts_failures(self) :
        """Tests 'pointer assertions in failure cases' """
        self.compile(prefix = 'test_pointer_asserts_failures', args = '--error-printer --world=UT_PointerAsserts_Failures', hppSources = ['UT_PointerAsserts_Failures.h'], cppSources = ['UT_PointerAsserts_Failures.cpp'], output = 'test_pointer_asserts_failures.out', compilerOptionalSwitches = self.std14Flags)

    def test_file_asserts_failures(self) :
        """Tests 'file assertions in failure cases' """
        self.compile(prefix = 'test_file_asserts_failures', args = '--error-printer --have-std --world=UT_FileAsserts_Failures', hppSources = ['UT_FileAsserts_Failures.h'], cppSources = ['UT_FileAsserts_Failures.cpp'], output = 'test_file_asserts_failures.out', compilerOptionalSwitches = self.haveStd)

    def test_file_asserts_successes(self) :
        """Tests 'file assertions' when all tests are successful """
        self.compile(prefix = 'test_file_asserts_successes', args = '--error-printer --have-std --world=UT_FileAsserts_Successes', hppSources = ['UT_FileAsserts_Successes.h'], cppSources = ['UT_FileAsserts_Successes.cpp'], output = 'test_file_asserts_successes.out', compilerOptionalSwitches = self.haveStd)

    def test_file_asserts_successes_details(self) :
        """Tests 'file assertions' when all tests are successful """
        self.compile(prefix = 'test_file_asserts_successes_details', runnerOptions = '--display_success', args = '--error-printer --have-std --world=UT_FileAsserts_Successes', hppSources = ['UT_FileAsserts_Successes.h'], cppSources = ['UT_FileAsserts_Successes.cpp'], output = 'test_file_asserts_successes_details.out', compilerOptionalSwitches = self.haveStd)

    def test_pointer_asserts_successes(self) :
        """Tests 'pointer assertions' when all tests are successful"""
        self.compile(prefix = 'test_pointer_asserts_successes', args = '--error-printer --world=UT_PointerAsserts_Successes', hppSources = ['UT_PointerAsserts_Successes.h'], cppSources = ['UT_PointerAsserts_Successes.cpp'], output = 'test_pointer_asserts_successes.out', compilerOptionalSwitches = self.std14Flags)

    def test_pointer_asserts_successes_details(self) :
        """Tests 'pointer assertions' when all tests are successful reporting all details """
        self.compile(prefix = 'test_pointer_asserts_successes_details', runnerOptions = '--display_success', args = '--error-printer --world=UT_PointerAsserts_Successes', hppSources = ['UT_PointerAsserts_Successes.h'], cppSources = ['UT_PointerAsserts_Successes.cpp'], output = 'test_pointer_asserts_successes_details.out', compilerOptionalSwitches = self.std14Flags)

    def test_data_driven_failures(self) :
        """Tests 'data driven assertion' failures """
        self.compile(prefix = 'test_data_driven_failures', args = '--error-printer --world=UT_DataDriven_Failures', hppSources = ['UT_DataDriven_Failures.h'], cppSources = ['UT_DataDriven_Failures.cpp'], output = 'test_data_driven_failures.out', compilerOptionalSwitches = self.std14Flags)


    def test_data_driven_successes(self) :
        """Tests 'data driven assertions when all tests are successful' """
        self.compile(prefix = 'test_data_driven_successes', args = '--error-printer --world=UT_DataDriven_Successes', hppSources = ['UT_DataDriven_Successes.h'], cppSources = ['UT_DataDriven_Successes.cpp'], output = 'test_data_driven_successes.out', compilerOptionalSwitches = self.std14Flags)

    def test_data_driven_successes_details(self) :
        """Tests 'data driven assertions' when all tests are successful reporting all details"""
        self.compile(prefix = 'test_data_driven_successes_details', runnerOptions = '--display_success', args = '--error-printer --world=UT_DataDriven_Successes', hppSources = ['UT_DataDriven_Successes.h'], cppSources = ['UT_DataDriven_Successes.cpp'], output = 'test_data_driven_successes_details.out', compilerOptionalSwitches = self.std14Flags)

class TestCpp(BaseTestCase, unittest.TestCase) :

    # Compiler specifics
    name           = 'c++'
    exe_option     = '-o'
    include_option = '-I'
    compiler       = 'c++ -Wall -W -Werror -g'
    no_eh_option   = None
    qtFlags        = '-Ifake'
    x11Flags       = '-Ifake'
    w32Flags       = '-Ifake'
    haveStd        = '-DCXXTEST_HAVE_STD'
    std11Flags     = '-std=c++11'
    std14Flags     = '-std=c++14'


    @property
    def DefinePrefix(self) :
        return "-D"

    logged = False;
    @staticmethod
    def logClassName() :
        if not __class__.logged :
            print ()
            print ("=========================")
            print ("======     c++     ======")
            print ("=========================")
            __class__.logged = True

    def init(self, prefix) :
        BaseTestCase.init(self, prefix)
        __class__.logClassName()

    def run(self, *args, **kwds) :
        if available(self, 'c++', '-o') :
            print ("")
            return unittest.TestCase.run(self, *args, **kwds)

    def setUp(self) :
        BaseTestCase.setUp(self)

    def tearDown(self) :
        BaseTestCase.tearDown(self)


class TestCppFOG(TestCpp) :

    fog = '-f'

    logged = False;
    @staticmethod
    def logClassName() :
        if not __class__.logged :
            print ()
            print ("=========================")
            print ("======   c++ Fog   ======")
            print ("=========================")
            __class__.logged = True

    def init(self, prefix) :
        BaseTestCase.init(self, prefix)
        __class__.logClassName()

    def run(self, *args, **kwds) :
        if ply_available :
            return TestCpp.run(self, *args, **kwds)
        else: printMissingPlyMessage()


class TestGpp(BaseTestCase, unittest.TestCase) :

    # Compiler specifics
    exe_option     = '-o'
    include_option = '-I'
    compiler       = 'g++ -g -Wmissing-declarations -Werror -Wno-error=long-long -Wall -W -Wshadow -Woverloaded-virtual -Wnon-virtual-dtor -Wreorder -Wsign-promo %s' % os.environ.get('CXXTEST_GCOV_FLAGS', '')
    no_eh_option   = '-fno-exceptions'
    qtFlags        = '-Ifake'
    x11Flags       = '-Ifake'
    w32Flags       = '-Ifake'
    haveStd        = '-DCXXTEST_HAVE_STD'
    std11Flags     = '-std=c++11'
    std14Flags     = '-std=c++14'

    @property
    def DefinePrefix(self) :
        return "-D"

    logged = False;
    @staticmethod
    def logClassName() :
        if not __class__.logged :
            print ()
            print ("=========================")
            print ("======   Gnu g++   ======")
            print ("=========================")
            __class__.logged = True

    def init(self, prefix) :
        BaseTestCase.init(self, prefix)
        __class__.logClassName()


    def run(self, *args, **kwds) :
        if available(self, 'g++', '-o') :
            print ("")
            return unittest.TestCase.run(self, *args, **kwds)

    def setUp(self) :
        BaseTestCase.setUp(self)

    def tearDown(self) :
        BaseTestCase.tearDown(self)


class TestGppPy(TestGpp) :

    def run(self, *args, **kwds) :
        if cxxtest_available :
            self.cxxtest_import = True
            status = TestGpp.run(self, *args, **kwds)
            self.cxxtest_import = False
            return status


class TestGppFOG(TestGpp) :
    fog = '-f'
    logged = False;
    @staticmethod
    def logClassName() :
        if not __class__.logged :
            print ()
            print ("=========================")
            print ("====== Gnu g++ Fog ======")
            print ("=========================")
            __class__.logged = True

    def init(self, prefix) :
        BaseTestCase.init(self, prefix)
        __class__.logClassName()


    def run(self, *args, **kwds) :
        if ply_available :
            return TestGpp.run(self, *args, **kwds)
        else: printMissingPlyMessage()


class TestGppFOGPy(TestGppFOG) :

    def run(self, *args, **kwds) :
        if cxxtest_available :
            self.cxxtest_import = True
            status = TestGppFOG.run(self, *args, **kwds)
            self.cxxtest_import = False
            return status


class TestGppValgrind(TestGpp) :

    valgrind = 'valgrind --tool=memcheck --leak-check=yes'

    def file_filter(self, file) :
        for line in file :
            if line.startswith('==') :
                continue
            # Some *very* old versions of valgrind produce lines like:
            #   free: in use at exit: 0 bytes in 0 blocks.
            #   free: 2 allocs, 2 frees, 360 bytes allocated.
            if line.startswith('free: ') :
                continue
            yield normalize_line_for_diff(line)

    def run(self, *args, **kwds) :
        if find('valgrind') is None :
            return
        return TestGpp.run(self, *args, **kwds)

    def parse_valgrind(self, fname) :
        # There is a well-known leak on Mac OSX platforms...
        if sys.platform == 'darwin' :
            min_leak = 16
        else :
            min_leak = 0
        #
        INPUT = open(fname, 'r')
        for line in INPUT :
            if not line.startswith('==') :
                continue
            tokens = re.split('[ \t]+', line)
            if len(tokens) < 4 :
                continue
            if tokens[1] == 'definitely' and tokens[2] == 'lost:' :
                if eval(tokens[3]) > min_leak :
                    self.fail("Valgrind Error: " + ' '.join(tokens[1 : ]))
            if tokens[1] == 'possibly' and tokens[2] == 'lost:' :
                if eval(tokens[3]) > min_leak :
                    self.fail("Valgrind Error: " + ' '.join(tokens[1 : ]))



class TestGppFOGValgrind(TestGppValgrind) :

    fog = '-f'

    def run(self, *args, **kwds) :
        if ply_available :
            return TestGppValgrind.run(self, *args, **kwds)
        else: printMissingPlyMessage()


class TestClang(BaseTestCase, unittest.TestCase) :

    # Compiler specifics
    exe_option     = '-o'
    include_option = '-I'
#+    compiler       = 'g++ -g -ansi -pedantic -Wmissing-declarations -Werror -Wall -W -Wshadow -Woverloaded-virtual -Wnon-virtual-dtor -Wreorder -Wsign-promo %s' % os.environ.get('CXXTEST_GCOV_FLAGS', '')
    compiler       = 'clang++ -v -g -Wall -W -Wshadow -Woverloaded-virtual -Wnon-virtual-dtor -Wreorder -Wsign-promo'
    #+ (begin JFC 15-July-2015): Hack for debug purpose
#+    compiler += ' -IC:\Qt\Qt5.5.0\Tools\mingw492_32\i686-w64-mingw32\include\c++'
    compiler += ' -IC:\Qt\Qt5.5.0\Tools\mingw492_32\i686-w64-mingw32\include -IC:\Qt\Qt5.5.0\Tools\mingw492_32\i686-w64-mingw32\include\c++ -IC:\Qt\Qt5.5.0\Tools\mingw492_32\i686-w64-mingw32\include\c++\i686-w64-mingw32'
    #+ (end   JFC 15-July-2015):
    no_eh_option   = '-fno-exceptions'
    qtFlags        = '-Ifake'
    x11Flags       = '-Ifake'
    w32Flags       = '-Ifake'
    haveStd        = '-DCXXTEST_HAVE_STD'
    std11Flags     = '-std=c++11'
    std14Flags     = '-std=c++14'

    @property
    def DefinePrefix(self) :
        return "-D"

    logged = False;
    @staticmethod
    def logClassName() :
        if not __class__.logged :
            print ()
            print ("=========================")
            print ("======   Clang++   ======")
            print ("=========================")
            __class__.logged = True

    def init(self, prefix) :
        BaseTestCase.init(self, prefix)
        __class__.logClassName()

    def run(self, *args, **kwds) :
        if available(self, 'clang++', '-o') :
            print ("")
            return unittest.TestCase.run(self, *args, **kwds)

    def setUp(self) :
        BaseTestCase.setUp(self)

    def tearDown(self) :
        BaseTestCase.tearDown(self)


class TestClangFOG(TestClang) :

    fog = '-f'

    logged = False;
    @staticmethod
    def logClassName() :
        if not __class__.logged :
            print ("=========================")
            print ("====== Clang++ Fog ======")
            print ("=========================")
            __class__.logged = True

    def init(self, prefix) :
        BaseTestCase.init(self, prefix)
        __class__.logClassName()

    def run(self, *args, **kwds) :
        if ply_available :
            return TestClang.run(self, *args, **kwds)
        else: printMissingPlyMessage()


class TestCL(BaseTestCase, unittest.TestCase) :

    # Compiler specifics
    exe_option     = '-Fe'
    include_option = '-I'
    #+ (begin JFC 15-July-2015): for debug purpose ==> Use environment variable to get include path
#+    includes       = '-I"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include"'
    includes       = ''
    #+ (end   JFC 15-July-2015):
    compiler       = 'cl -nologo -W4'
#+    compiler      += ' /MD'
    linkOptions    = ' /link  /ENTRY:main'
#+    linkOptions += ' /NODEFAULTLIB:libcmt'
    no_eh_option   = '-EHsc-'
    qtFlags        = '-Ifake'
    x11Flags       = '-Ifake'
    w32Flags       = '-Ifake'

    @property
    def DefinePrefix(self) :
        return "/D "

    logged = False;
    @staticmethod
    def logClassName() :
        if not __class__.logged :
            print ()
            print ("=========================")
            print ("======     Cl      ======")
            print ("=========================")
            __class__.logged = True

    def init(self, prefix) :
        BaseTestCase.init(self, prefix)
        __class__.logClassName()

    def getExePathSwitch(self, exeName) :
        return "{0}{1}".format(self.exe_option, exeName)

    def getIncludes(self) :
        includes = self.includes + BaseTestCase.getIncludes(self)
        return includes

    def getCompilerCommand(self, cppSources, haveExceptionHandling, compilerOptionalSwitches) :
        compilerOptionsAndSwitches = self.compiler;
        compilerOptionsAndSwitches += ' /EHsc /RTC1 /WX- /Gd /Gs /Oy- /Od /MDd /Fd"vc120.pdb"  /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /D "_UNICODE" /D "UNICODE"  '
#+        compilerOptionsAndSwitches += '    /D "_LIB"   '
#+        compilerOptionsAndSwitches += ' /GL /GR /GL /analyze- /Gy /Zc:wchar_t /Zi /Gm- /O2 /fp:precise /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_LIB" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /Gd /Oy- /Oi /MD /EHsc /nologo /Fp"activate_all.pch"'
#+        compilerOptionsAndSwitches += ' /MACHINE:X86 /SAFESEH /SUBSYSTEM:CONSOLE /TLBID:1'
        if haveExceptionHandling :
            compilerOptionsAndSwitches += " /EHsc " + self.DefinePrefix + "_CXXTEST_HAVE_EH"
            print ("compilerOptionsAndSwitches: {0}".format(compilerOptionsAndSwitches))

        exePathSwitch      = self.getExePathSwitch(self.build_target)
        includes           = self.getIncludes()
        allSourcesAsString = self.getAllSourcesAsString(cppSources)

        compilerCommand = "%s %s %s %s   %s %s > %s 2>&1" % (compilerOptionsAndSwitches, exePathSwitch, compilerOptionalSwitches, includes, allSourcesAsString, self.linkOptions, self.build_log)
        print ("compilerCommand: {0}".format(compilerCommand))
        return compilerCommand

    def run(self, *args, **kwds) :
        if available(self, self.compiler, self.exe_option, self.linkOptions) :
            print ("")
            return unittest.TestCase.run(self, *args, **kwds)

    def setUp(self) :
        BaseTestCase.setUp(self)

    def tearDown(self) :
        BaseTestCase.tearDown(self)


class TestCLFOG(TestCL) :
    fog = '-f'

    logged = False;
    @staticmethod
    def logClassName() :
        if not __class__.logged :
            print ()
            print ("=========================")
            print ("======   Cl Fog    ======")
            print ("=========================")
            __class__.logged = True

    def init(self, prefix) :
        BaseTestCase.init(self, prefix)
        __class__.logClassName()

    def run(self, *args, **kwds) :
        if ply_available :
            return TestCL.run(self, *args, **kwds)
        else: printMissingPlyMessage()

if __name__ == '__main__' :
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', '--verbose', dest = 'verbosity', action = 'store_const', const = 0, help = 'Verbose output')
    parser.add_argument('files', nargs = argparse.REMAINDER)
    param = parser.parse_args()
    unittest.main()

#===========================================================================
# End of test_cxxtest.py
#===========================================================================
