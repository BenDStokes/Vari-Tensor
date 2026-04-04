from conan import ConanFile
from conan.tools.build import can_run, check_min_cppstd
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy
from conan.tools.scm import Git
import os


class VariTensorConan(ConanFile):
    name = "Vari-Tensor"
    package_type = "library"
    version = "1.1.0"
    description = "Vari-Tensor is a library for ergonomic tensor algebra, including index variance."
    topics = ("tensors", "contravariant", "covariant", "metric", "differential geometry")
    license = "MPL-2.0"
    homepage = "https://github.com/BenDStokes/Vari-Tensor"

    settings = "os", "compiler", "build_type", "arch"

    options = {
        "linkage": ["static", "shared", "header_only"],
        "avx": [True, False],
        "validation": [True, False],
    }
    default_options = {
        "linkage": "static",
        "avx": False,
        "validation": True,
    }

    exports_sources = (
        "CMakeLists.txt",
        "Config.cmake.in",
        "src/*",
        "include/*",
        "varitensor.h",
        "test/*",
    )

    def source(self):
        git = Git(self)
        git.clone("https://github.com/BenDStokes/Vari-Tensor.git", target=".")
        git.checkout(f"main")

    def config_options(self):
        # AVX is a compilation option — irrelevant for the monolithic header
        if self.options.linkage == "header_only":
            del self.options.avx

    def configure(self):
        if self.options.linkage == "header_only":
            self.settings.rm_safe("compiler.libcxx")
            self.settings.rm_safe("compiler.cppstd")

    def package_id(self):
        # The header-only package is always the same file regardless of settings
        if self.options.linkage == "header_only":
            self.info.clear()

    def validate(self):
        check_min_cppstd(self, 23)

    def layout(self):
        cmake_layout(self)

    def generate(self):
        if self.options.linkage == "header_only":
            return
        tc = CMakeToolchain(self)
        tc.variables["BUILD_SHARED_LIBS"] = self.options.linkage == "shared"
        tc.variables["VARITENSOR_ENABLE_AVX"] = self.options.avx
        tc.generate()

    def build(self):
        if self.options.linkage == "header_only":
            return
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        if can_run(self):
            suffix = ""
            if self.options.get_safe("avx"):
                suffix += "_avx"
            if not self.options.validation:
                suffix += "_validation_off"
            bin_name = f"varitensor_tests{suffix}"
            self.run(os.path.join(self.build_folder, bin_name))

    def package(self):
        if self.options.linkage == "header_only":
            copy(self, "varitensor.h",
                 src=self.source_folder,
                 dst=os.path.join(self.package_folder, "include"))
        else:
            cmake = CMake(self)
            cmake.install()

    def package_info(self):
        if self.options.linkage == "header_only":
            self.cpp_info.bindirs = []
            self.cpp_info.libdirs = []
        else:
            self.cpp_info.libs = ["varitensor"]
            if self.options.avx:
                self.cpp_info.cxxflags = ["-mavx"]

        if not self.options.validation:
            self.cpp_info.defines = ["VARITENSOR_VALIDATION_ON=0"]
