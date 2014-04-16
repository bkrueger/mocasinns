"""
Python module for the statistical analysis of mocasinns-generated data using python.
"""

import numpy
import math

def array_to_dict(array, key_columns=[0], value_columns=[1]):
    """
    Convert an numpy array to a dictionary.
    If more than one key- or value-column is given, the key and/or the value will be stored as tuple

    Parameters
    ----------
    array : numpy array
        Array with data that should be converted to an dictionary
    key_columns : list of integers
        List of integers specifying the columns to use as keys; default = [0]
    value_columns: list of integers
        List of integers specifying the columns to use as values; default = [1]

    Examples and Tests
    ------------------
    >>> array_to_dict(numpy.loadtxt("test_data.dat")) == {1.0: 2.0, 4.0: 5.0, 7.0: 8.0}
    True
    """
    # Create dictionary
    if len(key_columns) == 1 and len(value_columns) == 1:
        return dict((array[i,key_columns[0]], array[i,value_columns[0]]) for i in range(array.shape[0]))
    if len(key_columns) == 1 and len(value_columns) > 1:
        return dict((array[i,key_columns[0]], tuple(array[i,value_columns])) for i in range(array.shape[0]))
    if len(key_columns) > 1 and len(value_columns) == 1:
        return dict((tuple(array[i,key_columns]), array[i,value_columns[0]]) for i in range(array.shape[0]))
    if len(key_columns) > 1 and len(value_columns) > 1:
        return dict((tuple(array[i,key_columns]), tuple(array[i,value_columns])) for i in range(array.shape[0]))

def loadtxt_dict(fname, key_columns=[0], value_columns=[1], dtype=float, comments='#', delimiter=None, skiprows=0):
    """
    Load data from a text file into a dictionary using the specified colums as key and values.
    If more than one key- or value-column is given, the key and/or the value will be stored as tuple
    
    Parameters
    ----------
    fname : file or str
        File, filename, or generator to read.  If the filename extension is
        ``.gz`` or ``.bz2``, the file is first decompressed. Note that
        generators should return byte strings for Python 3k.
    key_columns : list of integers
        List of integers specifying the columns to use as keys; default = [0]
    value_columns: list of integers
        List of integers specifying the columns to use as values; default = [1]
    dtype : data-type, optional
        Data-type of the resulting array; default: float.  If this is a
        record data-type, the resulting array will be 1-dimensional, and
        each row will be interpreted as an element of the array.  In this
        case, the number of columns used must match the number of fields in
        the data-type.
    comments : str, optional
        The character used to indicate the start of a comment;
        default: '#'.
    delimiter : str, optional
        The string used to separate values.  By default, this is any
        whitespace.
    skiprows : int, optional
        Skip the first `skiprows` lines; default: 0.

    Examples and Tests
    ------------------
    >>> loadtxt_dict("test_data.dat") == {1.0: 2.0, 4.0: 5.0, 7.0: 8.0}
    True
    >>> loadtxt_dict("test_data.dat", key_columns=[0], value_columns=[1,2]) == {1.0: (2.0, 3.0), 4.0: (5.0, 6.0), 7.0: (8.0, 9.0)}
    True
    >>> loadtxt_dict("test_data.dat", key_columns=[0,1], value_columns=[2]) == {(1.0, 2.0): 3.0, (4.0, 5.0): 6.0, (7.0, 8.0): 9.0}
    True
    
    """
    # Create the array columns that are important for the dictionary
    dict_columns = list(key_columns)
    dict_columns.extend(value_columns)
    # Use the numpy function to load the text to an array
    array = numpy.loadtxt(fname, dtype=dtype, comments=comments, delimiter=delimiter, skiprows=skiprows)
    # Convert the array to a dictionary
    return array_to_dict(array, key_columns=key_columns, value_columns=value_columns)

def dictionary_average(dicts, function=lambda x: (numpy.mean(x), numpy.std(x)/math.sqrt(x.size - 1))):
    """
    Create averaged dictionary from a list of several dictionaries.
    The averaging will be done element-wise.

    Parameters
    ----------
    dicts : list of dictionaries
        A list of dictionaries that should be averaged.
    function : callable
        Function that takes a numpy-array and calculates a 2-tuple consisting of the mean and the error of the array.
        The mocasinns.jackknife function can be used for this purpose.
        default: lambda x: (numpy.mean(x), numpy.std(x)/math.sqrt(x.size - 1))
    
    Examples and Tests
    ------------------
    >>> result_mean, result_error = dictionary_average([{2.0: 1.0, 4.0: 1.0}, {2.0: 2.0, 4.0: 3.0}, {2.0: 1.5}]) 
    >>> result_mean == {2.0: 1.5, 4.0: 2.0}, result_error == {2.0: math.sqrt(1.0/6.0) / math.sqrt(2), 4.0: 1.0}
    (True, True)
    """
    # Get all keys
    keys = set([])
    for dictionary in dicts:
        keys.update(dictionary.keys())
    # Create the compound dictionary
    dict_compound = {key: [] for key in keys}
    for dictionary in dicts:
        for key, value in dictionary.iteritems():
            dict_compound[key].append(value)
    # Average the compund dictionary to calculate the return dicitonaries
    dict_means = {}
    dict_errors = {}
    for key, value in dict_compound.iteritems():
        dict_means[key], dict_errors[key] = function(numpy.array(value))
    # Return the result
    return dict_means, dict_errors
        

def jackknife(a, function=lambda x: x, dtype=None):
    """
    Compute the jackknife mean and error of an array
    The average is taken over the flattened array by default.

    Parameters
    ----------
    a : array_like
        Array containing numbers whose jackknife mean and error is desired. 
        If `a` is not an array, a conversion is attempted.
    function: callable function
        (Non-linear) function that is applied to each jackknife mean value.
        Basically maps a numeric scalar to a numeric scalar.
        If an array is used as a parameter, the function must work on the array element-wise.
    dtype : data-type, optional
        Type to use in computing the jackknife mean and error. 
        For integer inputs, the default is `float64`; for floating point inputs, it is the same as the input dtype.

    Examples and Tests
    ------------------
    >>> a = numpy.array([1.0, 2.0, 3.0, 4.0, 5.0])
    >>> jackknife(a) == (3.0, math.sqrt(1.0/2.0))
    True
    >>> jackknife(a, function=lambda x:x**2) == (9.125, math.sqrt(2887.0/160.0))
    True
    >>> b = numpy.array([[1.0, 2.0, 3.0, 4.0, 5.0], [2.0, 3.0, 4.0, 5.0, 6.0]])
    >>> means_b, errors_b = jackknife(b)
    >>> (means_b == numpy.array([3.0, 4.0])).all()
    True
    >>> (errors_b == numpy.array([math.sqrt(1.0/2.0), math.sqrt(1.0/2.0)])).all()
    True
    """
    # Define the number of data points
    n = a.shape[-1]

    # Do the jackknifing for flat arrays
    if len(a.shape) == 1:
        jackknife_values = numpy.fromiter((function(numpy.mean(a[range(0, i) + range(i+1, n)], dtype=dtype)) for i in range(n)), numpy.float)
        return numpy.mean(jackknife_values), math.sqrt(n - 1)*numpy.std(jackknife_values)
    # Do the jackknifing for multi-dimensional arrays
    else:
        axis = len(a.shape) - 1
        jackknife_values = numpy.zeros(a.shape)
        for i in range(n):
            jackknife_values[...,i] = function(numpy.mean(a[..., range(0, i) + range(i+1, n)], axis=axis, dtype=dtype))
        return numpy.mean(jackknife_values, axis=axis), math.sqrt(n - 1)*numpy.std(jackknife_values, axis=axis)
        
def bootstrap(a, bootstrap_number, function=lambda x: x, dtype=None):
    """
    Compute the bootstrap mean and error of an array.
    The average is taken over the flattened array by default.
    For the use with multidimensional arrays see mocasinns.jackknife

    Parameters
    ----------
    a : array_like
        Array containing numbers whose jackknife mean and error is desired. 
        If `a` is not an array, a conversion is attempted.
    bootstrapnumber : integer
        Number of bootstrap resamplings
    function : callable function
        (Non-linear) function that is applied to each jackknife mean value
    dtype : data-type, optional
        Type to use in computing the jackknife mean and error. 
        For integer inputs, the default is `float64`; for floating point inputs, it is the same as the input dtype.

    Examples and Tests
    ------------------
    >>> a = numpy.random.normal(loc=5.0, scale=2.0, size=1000)
    >>> mean_a, error_a = bootstrap(a, 100)
    >>> (mean_a > 4.9, mean_a < 5.1)
    (True, True)
    >>> (error_a > 2.0/math.sqrt(1000 - 1) - 0.01, error_a < 2.0/math.sqrt(1000 - 1) + 0.01)
    (True, True)
    """
    # Define the number of data points
    n = a.shape[0]

    # Do the bootstrapping for flat arrays
    if len(a.shape) == 1:    
        bootstrap_values = numpy.fromiter((function(numpy.mean(a[numpy.random.randint(0, high=n, size=n)], dtype=dtype)) for i in range(bootstrap_number)), numpy.float)
        return numpy.mean(bootstrap_values), math.sqrt(float(bootstrap_number)/(bootstrap_number - 1))*numpy.std(bootstrap_values)
    # Do the bootstrapping for multi-dimensional arrays
    else:
        axis = len(a.shape) - 1
        bootstrap_values_shape = a.shape
        bootstrap_values_shape[-1] = bootstrap_number
        bootstrap_values = numpy.zeros(bootstrap_value_shape)
        for i in range(boostrap_number):
            bootstrap_values[..., i] = function(numpy.mean(a[..., numpy.random.randint(0, high=n, size=n)], axis=axis, dtype=dtype))
        return numpy.mean(bootstrap_values, axis=axis), math.sqrt(float(bootstrap_number)/(bootstrap_number - 1))*numpy.std(bootstrap_values, axis=axis)

def bootstrap_2(a, b, bootstrap_number, function=lambda a, b: a + b, dtype=None):
    """
    Compute the bootstrap mean and error of a function of two variables based on two observations a (for the first variable) and b (for the second variable)

    Parameters
    ----------
    a : array_like
        One-dimensional numpy array for data of the first kind
        If `a` is not an array, a conversion is attempted.
    b : array_like
        One-dimensional numpy array for data of the second kind
        If `b` is not an array, a conversion is attempted.
    bootstrapnumber : integer
        Number of bootstrap resamplings
    function : callable function
        Function of two parameters returning a numerical value for which the expectation value and the error should be calculated
    dtype : data-type, optional
        Type to use in computing the jackknife mean and error. 
        For integer inputs, the default is `float64`; for floating point inputs, it is the same as the input dtype.

    Examples and Tests
    ------------------
    >>> a = numpy.random.normal(loc=5.0, scale=2.0, size=1000)
    >>> b = numpy.random.normal(loc=3.0, scale=1.0, size=1000)
    >>> mean, error = bootstrap_2(a, b, 100)
    >>> (mean > 7.9, mean < 8.1)
    (True, True)
    >>> (error > math.sqrt(5.0)/math.sqrt(1000 - 1) - 0.01, error < math.sqrt(5.0)/math.sqrt(1000 - 1) + 0.01)
    (True, True)
    """
    # Define the number of data points
    n_a = a.size
    n_b = b.size
    
    # Do the bootstrapping
    bootstrap_values = numpy.fromiter((function(numpy.mean(a[numpy.random.randint(0, high=n_a, size=n_a)], dtype=dtype), numpy.mean(b[numpy.random.randint(0, high=n_b, size=n_b)], dtype=dtype)) for i in range(bootstrap_number)), numpy.float)
    return numpy.mean(bootstrap_values), math.sqrt(float(bootstrap_number)/(bootstrap_number - 1))*numpy.std(bootstrap_values)

def bootstrap_N(a, bootstrap_number, function=lambda x: numpy.sum(x), dtype=None):
    """
    Compute the expectation value and the error of a function of an arbitrary number of parameters, where there is an independent data set for each parameter

    Parameters
    ----------
    a : list of array_like
        List of N one-dimensional numpy arrays, the i-th list entry are the measurements for the i-th parameter of the function
        If the elements of `a` are not arrays, a conversion is attempted.
    bootstrapnumber : integer
        Number of bootstrap resamplings
    function : callable function
        Function of a list N of variables resulting in a scalar for which the expectation value and the error should be calculated
    dtype : data-type, optional
        Type to use in computing the jackknife mean and error. 
        For integer inputs, the default is `float64`; for floating point inputs, it is the same as the input dtype.

    Examples and Tests
    ------------------
    >>> a = numpy.random.normal(loc=5.0, scale=2.0, size=1000)
    >>> b = numpy.random.normal(loc=3.0, scale=1.0, size=1000)
    >>> mean, error = bootstrap_N([a, b], 100)
    >>> (mean > 7.9, mean < 8.1)
    (True, True)
    >>> (error > math.sqrt(5.0)/math.sqrt(1000 - 1) - 0.01, error < math.sqrt(5.0)/math.sqrt(1000 - 1) + 0.01)
    (True, True)
    """
    # Define the number of data points
    n = [x.size for x in a]
    
    # Do the bootstrapping
    bootstrap_values = numpy.fromiter((function([numpy.mean(a[i][numpy.random.randint(0, high=n[i], size=n[i])], dtype=dtype) for i in range(len(a))]) for j in range(bootstrap_number)), numpy.float)
    return numpy.mean(bootstrap_values), math.sqrt(float(bootstrap_number)/(bootstrap_number - 1))*numpy.std(bootstrap_values)

# Use doctest for unit tests
if __name__ == "__main__":
    import doctest
    doctest.testmod()
