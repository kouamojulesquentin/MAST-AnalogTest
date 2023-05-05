# MAST : Manager for SOC Test

MAST provides an implemenation of the IEEE 1687 and IEEE P1687.1 standard based on the handing of complex topologies and on the support for interactive, massively parallel routines. 
It does not have the vocation to be a complete reference implementation: as such, only a subset of features are fully implemented. However, the Kernel is optimized and highly flexibile, so for a user it will be easy to add teh desired features. 

## Compilation Details

MAST uses Cmake >= 3.82 for both compilation and Unit Testing. 

### Configuration 
MAST is a modular software, based on a Core and several Optional and External libraries their compilation and inclusion is defined by the UserOption.cmake file, not included in the distribution to avoid excessive versioning: you can build your own based on the  UserOption.model.cmake

### Compilation
To compile, simply type

```bash
make
```
This will configure, compile and install the "debug" build in the subdirectory ./cmake_debug. 


### Unitary Tests

Unitary Test can be launched with 

```bash
make test
```

Details of tests of the Mast_Core can be launched separately: 

```bash
make run_debug
```

## Usage

```python
import foobar

# returns 'words'
foobar.pluralize('word')

# returns 'geese'
foobar.pluralize('goose')

# returns 'phenomenon'
foobar.singularize('phenomena')
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[MIT](https://choosealicense.com/licenses/mit/)
