//
//  MCPolynomial.m
//  MCNumerics
//
//  Created by andrew mcknight on 12/13/13.
//  Copyright (c) 2013 andrew mcknight. All rights reserved.
//

#import "MCPolynomial.h"

#import "NSNumber+MCArithmetic.h"

@interface MCPolynomial ()

@end

@implementation MCPolynomial

#pragma mark - Init

- (id)initWithCoefficients:(NSArray *)coefficients
{
    self = [super init];
    if (self) {
        self.coefficients = coefficients;
    }
    return self;
}

+ (MCPolynomial *)polynomialWithCoefficients:(NSArray *)coefficients
{
    return [[MCPolynomial alloc] initWithCoefficients:coefficients];
}

#pragma mark - Inspection

- (BOOL)isEqualToPolynomial:(MCPolynomial *)otherPolynomial
{
    return [self.coefficients isEqualToArray:otherPolynomial.coefficients];
}

- (BOOL)isEqual:(id)object
{
    if (self == object) {
        return YES;
    }
    if (![object isKindOfClass:[MCPolynomial class]]) {
        return NO;
    }
    return [self isEqualToPolynomial:(MCPolynomial *)object];
}

- (NSUInteger)hash
{
    return [self.coefficients hash];
}

- (NSString *)description
{
    NSMutableString *description = [NSMutableString string];
    
    [self.coefficients enumerateObjectsUsingBlock:^(NSNumber *coefficient, NSUInteger power, BOOL *stop) {
        if (power == 0) {
            [description appendFormat:@"%.2f", coefficient.doubleValue];
        } else if (power == 1) {
            [description appendFormat:@"%.2f*x", coefficient.doubleValue];
        } else {
            [description appendFormat:@"%.2f*x^%u", coefficient.doubleValue, power];
        }
        
        if (power < self.coefficients.count) {
            [description appendString:@" + "];
        }
    }];
    
    return description;
}

#pragma mark - Operations

- (MCPolynomial *)derivativeOfDegree:(NSUInteger)degree
{
    NSMutableArray *derivativeCoefficients = [NSMutableArray array];
    NSIndexSet *coefficientsToEnumerate = [NSIndexSet indexSetWithIndexesInRange:NSMakeRange(degree, self.coefficients.count - degree)];
    [self.coefficients enumerateObjectsAtIndexes:coefficientsToEnumerate
                                         options:0
                                      usingBlock:^(NSNumber *coefficient, NSUInteger power, BOOL *stop) {
                                          NSNumber *derivativeCoefficient = coefficient;
                                          for (int i = 0; i < degree; i++) {
                                              derivativeCoefficient = [derivativeCoefficient productByMultiplying:@(power - i)];
                                          }
                                          [derivativeCoefficients addObject:derivativeCoefficient];
                                      }];
    return [MCPolynomial polynomialWithCoefficients:derivativeCoefficients];
}

- (NSNumber *)evaluateAtValue:(NSNumber *)value
{
    NSNumber __block *sum = @0.0;
    [self.coefficients enumerateObjectsUsingBlock:^(NSNumber *coefficient, NSUInteger power, BOOL *stop) {
        sum = [sum sumByAdding:[coefficient productByMultiplying:[value raiseToPower:@(power)]]];
    }];
    return sum;
}

- (NSNumber *)evaluateDerivativeOfDegree:(NSUInteger)degree withValue:(NSNumber *)value
{
    MCPolynomial *derivative = [self derivativeOfDegree:degree];
    return [derivative evaluateAtValue:value];
}

@end