# coroutine_lambdas_with_captures
**Demonstrates multiple incorrect ways, and one correct way, of capturing values in coroutine lambda functions**

### Purpose

The lifetime of captured values in coroutine lambdas is, unfortunately, quite unintuitive. Additionally, certain incorrect captures might seem to work with certain compilers regardless, possibly due to the underlying memory not being overwritten yet. Therefore, this project provides multiple examples, and is compatible with multiple compilers, to better understand these differences.

### Requirements
 
- C++20
- CMake 3.16

### Installation

Download only

### Tested on

- Visual Studio 16.10
- GCC 11.0 (WSL)
- Clang 12.0 (WSL)

### Download 

You can download the latest version of *coroutine_lambdas_with_captures* by cloning the GitHub repository:

	git clone https://github.com/HighDefinist/coroutine_lambdas_with_captures.git
	
### Usage

Open the folder in your favorite development environment (tested in Visual Studio and Visual Studio Code), or directly invoke CMake on the command line. 

### Additional notes and limitations

- The project contains a fallback mechanism to use <experimental/coroutine>, if <coroutine> is not found, including a namespace elevation from std::experimental to std. This approach should not be used in production code.
- I am not 100% confident about my explanation of why "AddGenAutoPass" fails. Also, there might be a way to modify this particular example to make it work which I am not aware of.
