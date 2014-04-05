//
//  MCSingularValueDecomposition.m
//  MCNumerics
//
//  Created by andrew mcknight on 12/15/13.
//  Copyright (c) 2013 andrew mcknight. All rights reserved.
//

#import <Accelerate/Accelerate.h>

#import "MCSingularValueDecomposition.h"
#import "MCMatrix.h"

@implementation MCSingularValueDecomposition

- (instancetype)initWithMatrix:(MCMatrix *)matrix
{
    self = [super init];
    if (self) {
        double workSize;
        double *work = &workSize;
        int lwork = -1;
        int m = matrix.rows;
        int n = matrix.columns;
        int numSingularValues = MIN(m, n);
        double *singularValues = malloc(numSingularValues * sizeof(double));
        int *iwork = malloc(8 * numSingularValues);
        int info = 0;
        
        double *values = malloc(m * n * sizeof(double));
        for (int i = 0; i < m * n; i++) {
            values[i] = matrix.values[i];
        }
        
        double *uValues = malloc(m * m * sizeof(double));
        double *vTValues = malloc(n * n * sizeof(double));
        double *sValues = malloc(m * n * sizeof(double));
        
        // call first with lwork = -1 to determine optimal size of working array
        dgesvd_("A", "A", &m, &n, values, &m, singularValues, uValues, &m, vTValues, &n, work, &lwork, &info);
        
        lwork = workSize;
        work = malloc(lwork * sizeof(double));
        
        // now run the actual decomposition
        dgesvd_("A", "A", &m, &n, values, &m, singularValues, uValues, &m, vTValues, &n, work, &lwork, &info);
        
        free(work);
        free(iwork);
        
        // build the sigma matrix
        int idx = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (i == j) {
                    sValues[idx] = singularValues[i];
                } else {
                    sValues[idx] = 0.0;
                }
                idx++;
            }
        }
        
        if (info == 0) {
            _u = [MCMatrix matrixWithValues:uValues rows:m columns:m];
            _vT = [MCMatrix matrixWithValues:vTValues rows:n columns:n];
            _s = [MCMatrix matrixWithValues:sValues rows:m columns:n];
        }
        
        free(singularValues);
        free(values);
    }
    return self;
}

+ (instancetype)singularValueDecompositionWithMatrix:(MCMatrix *)matrix
{
    return [[MCSingularValueDecomposition alloc] initWithMatrix:matrix];
}

#pragma mark - NSCopying

- (id)copyWithZone:(NSZone *)zone
{
    MCSingularValueDecomposition *svdCopy = [[self class] allocWithZone:zone];
    
    svdCopy->_s = _s.copy;
    svdCopy->_u = _u.copy;
    svdCopy->_vT = _vT.copy;
    
    return svdCopy;
}

@end
