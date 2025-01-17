//
//  MAVMatrix.h
//  MaVec
//
//  Created by andrew mcknight on 11/30/13.
//
//  Copyright (c) 2014 Andrew Robert McKnight
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.

//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//

#import <Foundation/Foundation.h>

#import "MAVTypedefs.h"

#import "NSNumber+MCKPrecision.h"

@class MAVSingularValueDecomposition;
@class MAVLUFactorization;
@class MAVQRFactorization;
@class MAVEigendecomposition;
@class MAVVector;
@class MCKTribool;

typedef enum : UInt8 {
    /**
     Specifies that this matrix' values are stored in row-major order.
     */
    MAVMatrixLeadingDimensionRow,
    
    /**
     Specifies that this matrix' values are stored in column-major order.
     */
    MAVMatrixLeadingDimensionColumn
}
/** 
 Constants specifying the leading dimension used for storing this matrix' values.
 */
MAVMatrixLeadingDimension;

typedef enum : UInt8 {
    /**
     Specifies that values refer to lower triangular portion.
     */
    MAVMatrixTriangularComponentLower,
    
    /**
     Specifies that values refer to upper triangular portion.
     */
    MAVMatrixTriangularComponentUpper,
    
    /**
     Special case where both or neither triangular components are defined.
     */
    MAVMatrixTriangularComponentBoth
}
/**
 Constants specifying the triangular portion of a square matrix.
 */
MAVMatrixTriangularComponent;

typedef enum : UInt8 {
    /**
     All values are flattened using a MAVMatrixLeadingDimension.
     
     @code
     [ a b
       c d ]   =>    [a b c d] or [a c b d]
     */
    MAVMatrixValuePackingMethodConventional,
    
    /**
     Exclude triangular matrix' leftover 0 values.
     
     @code
     [ a b c
       0 d e
       0 0 f ]  =>  [a b c d e f] or [a b d c e f]
     */
    MAVMatrixValuePackingMethodPacked,
    
    /**
     Only store the non-zero band values. MAVMatrixLeadingDimension has no bearing on this packing format.
     
     @code
     [ a b 0 0
       c d e 0
       f g h i
       0 j k l
       0 0 m n ]  ->  [* b e i a d h l c g k n f j m *] (* must exist in array but isn't used by the algorithm)
     */
    MAVMatrixValuePackingMethodBand
}
/**
 Constants specifying the matrix flattening method used to construct the values array.
 */
MAVMatrixValuePackingMethod;

typedef enum : UInt8 {
    /**
     x^TMx > 0 for all nonzero real x
     */
    MAVMatrixDefinitenessPositiveDefinite,
    
    /**
     x^TMx ≥ 0 for all real x
     */
    MAVMatrixDefinitenessPositiveSemidefinite,
    
    /**
     x^TMx < 0 for all nonzero real x
     */
    MAVMatrixDefinitenessNegativeDefinite,
    
    /**
     x^TMx ≤ 0 for all real x
     */
    MAVMatrixDefinitenessNegativeSemidefinite,
    
    /**
     x^TMx can be greater than, equal to or lesser than 0 for all real x
     */
    MAVMatrixDefinitenessIndefinite,
    
    /**
     Definiteness not yet been computed for this matrix.
     */
    MAVMatrixDefinitenessUnknown
}
/**
 Definiteness of a matrix M.
 */
MAVMatrixDefiniteness;

typedef enum : UInt8 {
    /**
     Specifies that an angle rotates in a clockwise direction when viewed in a right handed coordinate system.
     */
    MAVAngleDirectionClockwise,
    
    /**
     Specifies that an angle rotates in a counterclockwise direction when viewed in a right handed coordinate system.
     */
    MAVAngleDirectionCounterClockwise
}
/**
 Direction of an angle, either clockwise or counterclockwise when viewed in a right handed coordinate system. The actual coordinate system has no bearing on the values of a rotated vector.
 */
MAVAngleDirection;

typedef enum : UInt8 {
    /**
     The X cartesian axis.
     */
    MAVCoordinateAxisX,
    
    /**
     The Y cartesian axis.
     */
    MAVCoordinateAxisY,
    
    /**
     The Z cartesian axis.
     */
    MAVCoordinateAxisZ
}
/**
 One of the three Cartesian coordinate axes, either X, Y or Z.
 */
MAVCoordinateAxis;

/**
 @class MAVMatrix
 @description A class providing storage and operations for matrices of double-precision floating point numbers, where underlying details governing how the two-dimensional structure is reduced to the one-dimensional array containing its values (packing, leading dimension, or other internal value representation method) is abstracted away for any operation or property.
 */
@interface MAVMatrix : NSObject <NSCopying, NSMutableCopying>

/**
 @property rows 
 @brief The number of rows in the matrix.
 */
@property (nonatomic, readonly, assign) MAVIndex rows;

/**
 @property columns
 @brief The number of columns in the matrix.
 */
@property (nonatomic, readonly, assign) MAVIndex columns;

/**
 @property values
 @brief A one-dimensional C array of floating point values.
 */
@property (nonatomic, readonly, strong) NSData *values;

/**
 @property leadingDimension 
 @brief The leading dimension used to store this matrix' values in a one-dimensional array, either row- or column-major.
 */
@property (nonatomic, assign) MAVMatrixLeadingDimension leadingDimension;

/**
 @property packingMethod
 @brief The packing format used to store this matrix' values in a one-dimensional array, either conventional, packed or band.
 */
@property (nonatomic, assign) MAVMatrixValuePackingMethod packingMethod;

/**
 @property triangularComponent
 @brief The type of triangular matrix represented, either upper or lower, or both if the matrix is not strictly triangular.
 */
@property (nonatomic, readonly, assign) MAVMatrixTriangularComponent triangularComponent;

/**
 @property transpose
 @brief Transpose of this matrix. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) MAVMatrix *transpose;

/**
 @property determinant
 @brief The determinant of this matrix. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) NSNumber *determinant;

/**
 @property inverse
 @brief The (pseudo)inverse of this matrix. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) MAVMatrix *inverse;

/**
 @property minorMatrix
 @brief Each entry holds the value of the matrix minor from that point (the determinant of the submatrix formed by removing particular rows/columns; e.g. Mij of matrix A is the determinant of the submatrix of A without row i or column j). (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) MAVMatrix *minorMatrix;

/**
 @property cofactorMatrix
 @brief Each entry holds the cofactor of the matrix from that point (Cij of matrix A is the cofactor obtained by multiplying the minor at the same point by (-1)^(i+j) ). (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) MAVMatrix *cofactorMatrix;

/**
 @property adjugate
 @brief The adjugate matrix is the transpose of cofactorMatrix. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) MAVMatrix *adjugate;

/**
 @property conditionNumber
 @brief The condition number of this matrix. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) NSNumber *conditionNumber;

/**
 @property qrFactorization
 @brief An MAVQRFactorization object containing matrices representing the QR factorization of this matrix. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) MAVQRFactorization *qrFactorization;

/**
 @property luFactorization
 @description Helpful documentation at http://www.netlib.no/netlib/lapack/double/dgetrf.f and https://publib.boulder.ibm.com/infocenter/clresctr/vxrx/index.jsp?topic=%2Fcom.ibm.cluster.essl.v5r2.essl100.doc%2Fam5gr_hsgetrf.htm
 @brief An MAVLUFactorization object contatining matrices representing the LU factorization of this matrix. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) MAVLUFactorization *luFactorization;

/**
 @property singularValueDecomposition
 @description Uses the Accelerate framework function dgesdd_. Examples of dgesdd_(...) usage found at http://software.intel.com/sites/products/documentation/doclib/mkl_sa/11/mkl_lapack_examples/lapacke_dgesdd_row.c.htm and http://stackoverflow.com/questions/5047503/lapack-svd-singular-value-decomposition Good documentation exists at http://www.netlib.org/lapack/lug/node53.html and http://www.nag.com/numeric/FL/nagdoc_fl22/xhtml/F08/f08kdf.xml. See http://www.netlib.org/lapack/lug/node38.html for general documentation.
 @brief An MAVSingularValueDecomposition object containing matrices representing the singular value decomposition of this matrix, or nil if no such decomposition exists. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) MAVSingularValueDecomposition *singularValueDecomposition;

/**
 @property eigendecomposition
 @brief Computes the eigendecomposition (spectral factorization) of this matrix. Documentation found at ... . (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) MAVEigendecomposition *eigendecomposition;

/**
 @property isSymmetric
 @brief YES if this matrix is symmetric, NO otherwise. Default value = MCKTriboolValueUnknown. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong, getter=isSymmetric) MCKTribool *symmetric;

/**
 *  @property isIdentity
 *  @brief YES if this matrix is the identity matrix, NO otherwise. Default value = MCKTriboolValueUnknown. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) MCKTribool *isIdentity;

/**
 *  @property isZero
 *  @brief YES if this matrix is the zero matrix, NO otherwise. Default value = MCKTriboolValueUnknown. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) MCKTribool *isZero;

/**
 @property definiteness
 @brief The definiteness enum value for this matrix. Default value = MAVMatrixDefinitenessUnknown. (Lazy-loaded)
 */
@property (nonatomic, readonly, assign) MAVMatrixDefiniteness definiteness;

/**
 @property diagnoalValues
 @brief A vector containing the values on the main diagonalfrom top to bottom. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) MAVVector *diagonalValues;

/**
 @property trace
 @brief The sum of the values on the main diagonal. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) NSNumber *trace;

/**
 @property normL1
 @brief The maximum absolute column sum of the matrix. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) NSNumber *normL1;

/**
 @property normInfinity
 @brief The maximum absolute row sum of the matrix. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) NSNumber *normInfinity;

/**
 @property normMax
 @brief The maximum value of all entries in the matrix. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) NSNumber *normMax;

/**
 @property normFroebenius
 @brief Square root of the sum of the squared values in the matrix. (Lazy-loaded)
 */
@property (nonatomic, readonly, strong) NSNumber *normFroebenius;

/**
 @property precision
 @brief The precision of the numeric values in the matrix, either single- or double-precision floating point.
 */
@property (nonatomic, readonly, assign) MCKPrecision precision;


#pragma mark - Constructors

/**
 @brief Construct a new MAVMatrix object.
 @param values A one-dimensional array of floating-point values.
 @param rows The amount of rows the matrix should have.
 @param columns The amount of columns the matrix should have.
 @param leadingDimension The leading dimension that should be used when inspecting the supplied values parameter.
 @param packingMethod Describes how the values are packed in the supplied values parameter.
 @param triangularComponent Describes the triangular component described by the supplied values parameter.
 @return A new MAVMatrix object.
 */
- (instancetype)initWithValues:(NSData *)values
                          rows:(MAVIndex)rows
                       columns:(MAVIndex)columns
              leadingDimension:(MAVMatrixLeadingDimension)leadingDimension
                 packingMethod:(MAVMatrixValuePackingMethod)packingMethod
           triangularComponent:(MAVMatrixTriangularComponent)triangularComponent;

#pragma mark - Class constructors

/**
 @brief Class convenience method to create a matrix with the specified number of rows and columns but without supplying values.
 @description  Instantiates a new object of type MAVMatrix with the specified number of rows and columns, with no supplied values, but with an initialized array to hold those values stored in column-major format and in the specified floating-point precision.
 @param rows The number of rows.
 @param columns The number of columns.
 @param precision The precision the matrix values will be stored in.
 @return New instance of MAVMatrix.
 */
+ (instancetype)matrixWithRows:(MAVIndex)rows
                       columns:(MAVIndex)columns
                     precision:(MCKPrecision)precision;

/**
 @brief Class convenience method to create a matrix with the specified number of rows and columns and storage format, but without supplying values.
 @description  Instantiates a new object of type MAVMatrix with the specified number of rows and columns, with no supplied values, but with an initialized array to hold those values stored in the specified format and specified floating-point precision..
 @param rows The number of rows.
 @param columns The number of columns.
 @param precision The precision the matrix values will be stored in.
 @param leadingDimension The format to store values in; either row- or column-major.
 @return New instance of MAVMatrix.
 */
+ (instancetype)matrixWithRows:(MAVIndex)rows
                       columns:(MAVIndex)columns
                     precision:(MCKPrecision)precision
              leadingDimension:(MAVMatrixLeadingDimension)leadingDimension;

/**
 @brief Class convenience method to create a matrix with the specified values and number of rows and columns.
 @description  Instantiates a new object of type MAVMatrix with the specified number of rows and columns and supplied values in column-major format.
 @param values The values to store in this matrix, supplied as a C array.
 @param rows The number of rows.
 @param columns The number of columns.
 @return New instance of MAVMatrix.
 */
+ (instancetype)matrixWithValues:(NSData *)values
                            rows:(MAVIndex)rows
                         columns:(MAVIndex)columns;

/**
 @brief Class convenience method to create a matrix with the specified values (in the specified storage format) and number of rows and columns.
 @description  Instantiates a new object of type MAVMatrix with the specified number of rows and columns and supplied values in the specified format.
 @param values The values to store in this matrix, supplied as a C array.
 @param rows The number of rows.
 @param columns The number of columns.
 @param leadingDimension The format to store values in; either row- or column-major.
 @return New instance of MAVMatrix.
 */
+ (instancetype)matrixWithValues:(NSData *)values
                            rows:(MAVIndex)rows
                         columns:(MAVIndex)columns
                leadingDimension:(MAVMatrixLeadingDimension)leadingDimension;

/**
   @brief Class convenience method to create a general matrix filled with a single supplied value.

   @param value   The value to fill the matrix with.
   @param rows    Amount of rows in the matrix.
   @param columns Amount of rows in the matrix.

   @return New instance of MAVMatrix filled with the supplied value.
 */
+ (instancetype)matrixFilledWithValue:(NSNumber *)value
                                 rows:(MAVIndex)rows
                              columns:(MAVIndex)columns;

/**
 @brief Class convenience method to create a triangular matrix filled with a single supplied value.

 @param value               The value to fill the matrix with.
 @param order               Amount of rows/columns in the square matrix.
 @param triangularComponent Either upper or lower, specifying the part of the matrix to fill with the supplied value.

 @return New triangular MAVMatrix with specified component filled with the supplied value.
 */
+ (instancetype)triangularMatrixFilledWithValue:(NSNumber *)value
                                          order:(MAVIndex)order
                            triangularComponent:(MAVMatrixTriangularComponent)triangularComponent;

/**
 @brief Class convenience method to create a band matrix filled with a single supplied value.

 @param value            The value to fill the matrix with.
 @param order            Amount of rows/columns in the square matrix.
 @param upperCodiagonals Amount of codiagonals above the diagonal to fill with the value.
 @param lowerCodiagonals Amount of codiagonals below the diagonal to fill with the value.

 @return New band MAVMatrix with band filled with the supplied value.
 */
+ (instancetype)bandMatrixFilledWithValue:(NSNumber *)value
                                    order:(MAVIndex)order
                         upperCodiagonals:(MAVIndex)upperCodiagonals
                         lowerCodiagonals:(MAVIndex)lowerCodiagonals;

/**
 @brief Class convenience method to create a square identity matrix with the specified size.
 @description  Instantiates a new object of type MAVMatrix with dimensions size x size whose diagonal values are 1.0 and all other values are 0.0, stored in column-major format.
 @param order The square dimension in which to create this identity matrix.
 @param precision The precision of the floating point values.
 @return New instance of MAVMatrix representing the identity matrix of dimension size x size.
 */
+ (instancetype)identityMatrixOfOrder:(MAVIndex)order
                            precision:(MCKPrecision)precision;

/**
 @brief Class convenience method to create a square matrix with the specified diagonal values.
 @description  Instantiates a new object of type MAVMatrix of dimension size x size with the specified diagonal values and other values as 0.0, stored in column-major format.
 @param values The values for the diagonal of the matrix, from the top-leftmost value to the bottom-rightmost value.
 @param order The square dimension inwhich to create this diagonal matrix.
 @return New instance of MAVMatrix representing the square matrix of dimension size x size with specified diagonal values.
 */
+ (instancetype)diagonalMatrixWithValues:(NSData *)values
                                   order:(MAVIndex)order;

/**
 @brief Class convenience method to create a matrix from an array of MAVVectors describing the matrix column vectors.
 @param columnVectors The array of MAVVector objects describing the columns of the matrix.
 @return A new instance of MAVMatrix.
 */
+ (instancetype)matrixWithColumnVectors:(NSArray *)columnVectors;

/**
 @brief Class convenience method to create a matrix from an array of MAVVectors describing the matrix row vectors.
 @param rowVectors The array of MAVVector objects describing the rows of the matrix.
 @return A new instance of MAVMatrix.
 */
+ (instancetype)matrixWithRowVectors:(NSArray *)rowVectors;

/**
 @brief Class convenience method to create a square matrix from an array of values describing a triangular component of a matrix, either upper or lower.
 @param values The array of values in the matrix.
 @param triangularComponent Which triangular component the provided values belong to. (Cannot be MAVMatrixTriangularComponentBoth)
 @param leadingDimension The leading dimension to use when accessing values in the provided one-dimensional array.
 @param order The number of rows/columns in the square triangular matrix.
 @return A new instance of MAVMatrix representing the desired triangular matrix.
 */
+ (instancetype)triangularMatrixWithPackedValues:(NSData *)values
                           ofTriangularComponent:(MAVMatrixTriangularComponent)triangularComponent
                                leadingDimension:(MAVMatrixLeadingDimension)leadingDimension
                                           order:(MAVIndex)order;

/**
 @brief Class convenience method to create a square symmetric from an array of values describing a triangular component of a matrix, either upper or lower.
 @param values The array of values in the matrix.
 @param triangularComponent Which triangular component the provided values belong to. (Cannot be MAVMatrixTriangularComponentBoth)
 @param leadingDimension The leading dimension to use when accessing values in the provided one-dimensional array.
 @param order The number of rows/columns in the square triangular matrix.
 @return A new instance of MAVMatrix representing the desired triangular matrix.
 */
+ (instancetype)symmetricMatrixWithPackedValues:(NSData *)values
                            triangularComponent:(MAVMatrixTriangularComponent)triangularComponent
                               leadingDimension:(MAVMatrixLeadingDimension)leadingDimension
                                          order:(MAVIndex)order;

/**
 @brief Class convenience method to create a square band matrix with supplied (co)diagonal values in band matrix format (see http://www.roguewave.com/Portals/0/products/imsl-numerical-libraries/c-library/docs/6.0/math/default.htm?turl=matrixstoragemodes.htm for a good explanation).
 @param values Values in the bands of the matrix supplied in the band format.
 @param order Number of rows/columns in the matrix.
 @param bandwidth Number of diagonals and codiagonals in the band matrix.
 @param oddDiagonalLocation If there is an extra codiagonal, specifies whether it appears in the upper or lower triangular component of the matrix; disregarded otherwise. (Cannot be MAVMatrixTriangularComponentBoth if bandwidth is even).
 @return A new instance of MAVMatrix representing the band matrix.
 */
+ (instancetype)bandMatrixWithValues:(NSData *)values
                               order:(MAVIndex)order
                    upperCodiagonals:(MAVIndex)upperCodiagonals
                    lowerCodiagonals:(MAVIndex)lowerCodiagonals;

/**
 @brief Class convenience method to create a matrix describing the rotation of a vector through a fixed two dimensional Cartesian space.
 @param angle The magnitude of the angle.
 @param direction The direction of the angle, either clockwise or counterclockwise.
 @return A new instance of MAVMatrix representing the rotation matrix.
 */
+ (instancetype)matrixForTwoDimensionalRotationWithAngle:(NSNumber *)angle direction:(MAVAngleDirection)direction;

/**
 @brief Class convenience method to create a matrix describing the rotation of a vector about a single axis of a fixed three dimensional Cartesian space.
 @param angle The magnitude of the angle.
 @param axis The three dimensional Cartesian axis to rotate about, either X, Y or Z.
 @param direction The direction of the angle, either clockwise or counterclockwise, when the positive end of the axis faces the observer.
 @return A new instance of MAVMatrix representing the rotation matrix.
 */
+ (instancetype)matrixForThreeDimensionalRotationWithAngle:(NSNumber *)angle
                                                 aboutAxis:(MAVCoordinateAxis)axis
                                                 direction:(MAVAngleDirection)direction;

/**
 @brief Class convenience method to create a matrix with the specified size containing random double-precision floating-point values.
 @param rows The number of rows desired in the random matrix.
 @param columns The number of columns desired in the random matrix.
 @param precision The precision of the floating point values.
 @return A new instance of MAVMatrix containing rows * columns random values.
 */
+ (instancetype)randomMatrixWithRows:(MAVIndex)rows
                             columns:(MAVIndex)columns
                           precision:(MCKPrecision)precision;

/**
 @brief Class convenience method to create a square symmetric matrix with the specified order containing random double-precision floating-point values.
 @param order The amount of rows/columns desired in the matrix.
 @param precision The precision of the floating point values.
 @return A new square symmetric instance of MAVMatrix containing random values.
 */
+ (instancetype)randomSymmetricMatrixOfOrder:(MAVIndex)order
                                   precision:(MCKPrecision)precision;

/**
 @brief Class convenience method to create a square diagonal matrix with the specified order containing random double-precision floating-point values.
 @param order The amount of rows/columns desired in the matrix.
 @param precision The precision of the floating point values.
 @return A new square diagonal instance of MAVMatrix containing random values.
 */
+ (instancetype)randomDiagonalMatrixOfOrder:(MAVIndex)order
                                  precision:(MCKPrecision)precision;

/**
 @brief Class convenience method to create a square triangular matrix with the specified order containing random double-precision floating-point values.
 @param order The amount of rows/columns desired in the matrix.
 @param triangularComponent The triangular component the values should reside in, either upper or lower (cannot be MAVMatrixTriangularComponentBoth).
 @param precision The precision of the floating point values.
 @return A new square triangular instance of MAVMatrix containing random values.
 */
+ (instancetype)randomTriangularMatrixOfOrder:(MAVIndex)order
                          triangularComponent:(MAVMatrixTriangularComponent)triangularComponent
                                    precision:(MCKPrecision)precision;

/**
 @brief Class convenience method to create a square band matrix with the specified order containing random double-precision floating-point values.
 @param order The amount of rows/columns desired in the matrix.
 @param bandwidth Number of diagonals and codiagonals in the band matrix.
 @param oddDiagonalLocation If there is an extra codiagonal, specifies whether it appears in the upper or lower triangular component of the matrix; disregarded otherwise. (Cannot be MAVMatrixTriangularComponentBoth if bandwidth is even).
 @param precision The precision of the floating point values.
 @return A new square band instance of MAVMatrix containing random values.
 */
+ (instancetype)randomBandMatrixOfOrder:(MAVIndex)order
                       upperCodiagonals:(MAVIndex)upperCodiagonals
                       lowerCodiagonals:(MAVIndex)lowerCodiagonals
                              precision:(MCKPrecision)precision;

/**
 @brief Generate a square matrix of random values with the specified definiteness and precision. Random semidefinite matrices are currently generated as diagonal matrices, the others have random values throughout.
 @param order The square dimension of the matrix to be generated.
 @param definiteness The definiteness of the matrix to generate, either positive defininte, positivie semidefinite, negative definite, negative semidefinite, or indefinite. (Passing MAVMatrixDefinitenessUnknown will simply return a random matrix generated by randomMatrixWithRows:columns:precision:)
 @param precision The precision of the random floating point values to generate, either single- or double-precision.
 @return A square MAVMatrix object containing random floating point values of desired precision that satisfies the specified definiteness criteria.
 */
+ (instancetype)randomMatrixOfOrder:(MAVIndex)order
                       definiteness:(MAVMatrixDefiniteness)definiteness
                          precision:(MCKPrecision)precision;

/**
 @brief Generate a square matrix of random values with a determinant of 0, accomplished by making an entire row or column (randomly chosen) with 0 values.
 @param order The square dimension of the matrix.
 @param precision Either single- or double-precision floating point values.
 @return New instance of MAVMatrix with random values and determinant == 0.
 */
+ (instancetype)randomSingularMatrixOfOrder:(MAVIndex)order precision:(MCKPrecision)precision;

/**
 @brief Generate a square matrix of random values with a determinant not equal to 0.
 @param order The square dimension of the matrix.
 @param precision Either single- or double-precision floating point values.
 @return New instance of MAVMatrix with random values and determinant != 0.
 */
+ (instancetype)randomNonsigularMatrixOfOrder:(MAVIndex)order precision:(MCKPrecision)precision;

#pragma mark - NSObject overrides

/**
 @return YES if otherMatrix is either this MAVMatrix instance or is identical in dimension and contains identical values at all positions, NO otherwise.
 */
- (BOOL)isEqualToMatrix:(MAVMatrix *)otherMatrix;

/**
 @return YES if object is either this MAVMatrix instance or is identical in dimension and contains identical values at all positions, NO otherwise.
 */
- (BOOL)isEqual:(id)object;

/**
 @return An NSString that can represent the values of this matrix in the usual two-dimensional human-readable format.
 */
- (NSString *)description;

- (id)debugQuickLookObject;

#pragma mark - Inspection

/**
 @brief Return the values in this matrix represented in a one-dimensional array flattened using the specified leading dimension.
 @param leadingDimension Dimension to use when flattening the values into a one-dimensional array.
 @return A copy of this matrix' values stored in the specified format (row-major or column-major).
 */
- (NSData *)valuesWithLeadingDimension:(MAVMatrixLeadingDimension)leadingDimension;

/**
 @brief Return values from the specified triangular component of the matrix, flattened into a one-dimensional array using the specified leading dimension and packing format.
 @param triangularComponent The desired triangular component to extract values from (cannot be MAVMatrixTriangularComponentBoth).
 @param leadingDimension The leading dimension to consider when flattening the values into the array.
 @param packingMethod The packing format to consider when flattening the values into the array.
 @return A copy of this matrix' values from the specified triangular component, flattened and packed into a one-dimensional according to specified parameters.
 */
- (NSData *)valuesFromTriangularComponent:(MAVMatrixTriangularComponent)triangularComponent
                         leadingDimension:(MAVMatrixLeadingDimension)leadingDimension
                            packingMethod:(MAVMatrixValuePackingMethod)packingMethod;

/**
 @brief Return the values in the band between a specified upper and lower codiagonal.
 @param upperCodiagonal The codiagonal above the main diagonal to use as a boundary for the band.
 @param lowerCodiagonal The codiagonal below the main diagonal to use as a boundary for the band.
 @return Pointer to an array of band-format values.
 */
- (NSData *)valuesInBandBetweenUpperCodiagonal:(MAVIndex)upperCodiagonal
                               lowerCodiagonal:(MAVIndex)lowerCodiagonal;

/**
 @description Get the value at a position specified by row and column. Raises an NSRangeException if the position does not exist in the matrix.
 @param row The row in which the desired value resides.
 @param column The column in which the desired value resides.
 @return The value at the specified row and column.
 */
- (NSNumber *)valueAtRow:(MAVIndex)row column:(MAVIndex)column;

/**
 @brief Extract the values of a column of this matrix.
 @param column The index of the column to extract.
 @return An MAVVector object contaning the values in the specified column.
 */
- (MAVVector *)columnVectorForColumn:(MAVIndex)column;

/**
 @brief Extract the values of a row of this matrix.
 @param column The index of the row to extract.
 @return An MAVVector object contaning the values in the specified row.
 */
- (MAVVector *)rowVectorForRow:(MAVIndex)row;

/**
 @return An array containing MAVVector objects representing the row vectors of the matrix from top to bottom.
 */
- (NSArray *)rowVectors;

/**
 @return An array containing MAVVector objects representing the column vectors of the matrix from left to right.
 */
- (NSArray *)columnVectors;

#pragma mark - Subscripting

/**
 @brief Extract the values of a row of this matrix, using overridden bracket operators.
 @param column The index of the row to extract.
 @return An MAVVector object containing the values in the specified row.
 */
- (MAVVector *)objectAtIndexedSubscript:(MAVIndex)idx;

#pragma mark - Class-level operations

/**
 @description Good documentation for solving Ax=b where A is a square matrix located  at http://www.netlib.org/lapack/double/dgesv.f and example at http://software.intel.com/sites/products/documentation/doclib/mkl_sa/11/mkl_lapack_examples/dgesv_ex.c.htm. When A is a general m x n matrix, see documentation at http://www.netlib.org/lapack/double/dgels.f and example at http://software.intel.com/sites/products/documentation/doclib/mkl_sa/11/mkl_lapack_examples/dgels_ex.c.htm
 @return A column vector containing coefficients for unknows to solve a linear system Ax=B, or nil if the system cannot be solved. Raises an NSInvalidArgumentException if A and B are of incompatible dimension.
 */
+ (MAVVector *)solveLinearSystemWithMatrixA:(MAVMatrix *)A
                                    valuesB:(MAVVector *)B;

/**
 @brief Multiplies an array of matrices together. Uses the Hu-Shing polygon partitioning method to determine the optimum order of multiplication to minimize the amount of operations. http://www.cs.ust.hk/mjg_lib/bibs/DPSu/DPSu.Files/0213017.pdf
 @param matrices An NSArray of MAVMatrix objects.
 @return A new MAVMatrix object holding the result of the multiplication.
 */
+ (MAVMatrix *)productOfMatrices:(NSArray *)matrices;

@end
