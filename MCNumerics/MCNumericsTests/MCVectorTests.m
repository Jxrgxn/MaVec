//
//  MCVectorTests.m
//  MCNumerics
//
//  Created by andrew mcknight on 2/16/14.
//  Copyright (c) 2014 andrew mcknight. All rights reserved.
//

#import <XCTest/XCTest.h>

#import "MCVector.h"

@interface MCVectorTests : XCTestCase

@end

@implementation MCVectorTests

- (void)setUp
{
    [super setUp];
    // Put setup code here; it will be run once, before the first test case.
}

- (void)tearDown
{
    // Put teardown code here; it will be run once, after the last test case.
    [super tearDown];
}

- (void)testVectorDotProduct
{
    double dotProduct = [MCVector dotProductOfVectorA:[MCVector vectorWithValuesInArray:@[
                                                                                          @1,
                                                                                          @3,
                                                                                          @(-5)]]
                                           andVectorB:[MCVector vectorWithValuesInArray:@[
                                                                                          @4,
                                                                                          @(-2),
                                                                                          @(-1)]]];
    XCTAssertEqual(dotProduct, 3.0, @"Dot product not computed correctly");
    
    dotProduct = [MCVector dotProductOfVectorA:[MCVector vectorWithValuesInArray:@[
                                                                                   @0,
                                                                                   @0,
                                                                                   @1]]
                                    andVectorB:[MCVector vectorWithValuesInArray:@[
                                                                                   @0,
                                                                                   @1,
                                                                                   @0]]];
    XCTAssertEqual(dotProduct, 0.0, @"Dot product not computed correctly");
    
    @try {
        dotProduct = [MCVector dotProductOfVectorA:[MCVector vectorWithValuesInArray:@[
                                                                                       @0,
                                                                                       @0,
                                                                                       @1]]
                                        andVectorB:[MCVector vectorWithValuesInArray:@[
                                                                                       @0,
                                                                                       @1,
                                                                                       @0,
                                                                                       @1]]];
    }
    @catch (NSException *exception) {
        XCTAssert([exception.name isEqualToString:NSInvalidArgumentException], @"Did not detect dimension mismatch in MCVector dot product method");
    }
}

- (void)testVectorAddition
{
    MCVector *a = [MCVector vectorWithValuesInArray:@[@1, @2, @3, @4]];
    MCVector *b = [MCVector vectorWithValuesInArray:@[@5, @6, @7, @8]];
    MCVector *c = [MCVector vectorWithValuesInArray:@[@1, @2, @3]];
    
    MCVector *sum = [MCVector sumOfVectorA:a andVectorB:b];
    MCVector *solution = [MCVector vectorWithValuesInArray:@[@6, @8, @10, @12]];
    for (int i = 0; i < 4; i++) {
        XCTAssertEqual([sum valueAtIndex:i], [solution valueAtIndex:i], @"Value at index %u not added correctly", i);
    }
    
    XCTAssertThrows([MCVector sumOfVectorA:a andVectorB:c], @"Should throw a mismatched dimension exception");
}

- (void)testVectorSubtraction
{
    MCVector *a = [MCVector vectorWithValuesInArray:@[@1, @2, @3, @4]];
    MCVector *b = [MCVector vectorWithValuesInArray:@[@5, @6, @7, @8]];
    MCVector *c = [MCVector vectorWithValuesInArray:@[@1, @2, @3]];
    
    MCVector *diff = [MCVector differenceOfVectorA:b andVectorB:a];
    MCVector *solution = [MCVector vectorWithValuesInArray:@[@4, @4, @4, @4]];
    for (int i = 0; i < 4; i++) {
        XCTAssertEqual([diff valueAtIndex:i], [solution valueAtIndex:i], @"Value at index %u not subtracted correctly", i);
    }
    
    XCTAssertThrows([MCVector differenceOfVectorA:a andVectorB:c], @"Should throw a mismatched dimension exception");
}

- (void)testVectorMultiplication
{
    MCVector *a = [MCVector vectorWithValuesInArray:@[@1, @2, @3, @4]];
    MCVector *b = [MCVector vectorWithValuesInArray:@[@5, @6, @7, @8]];
    MCVector *c = [MCVector vectorWithValuesInArray:@[@1, @2, @3]];
    
    MCVector *prod = [MCVector productOfVectorA:a andVectorB:b];
    MCVector *solution = [MCVector vectorWithValuesInArray:@[@5, @12, @21, @32]];
    for (int i = 0; i < 4; i++) {
        XCTAssertEqual([prod valueAtIndex:i], [solution valueAtIndex:i], @"Value at index %u not multiplied correctly", i);
    }
    
    XCTAssertThrows([MCVector productOfVectorA:a andVectorB:c], @"Should throw a mismatched dimension exception");
}

- (void)testVectorDivision
{
    MCVector *a = [MCVector vectorWithValuesInArray:@[@1, @2, @3, @4]];
    MCVector *b = [MCVector vectorWithValuesInArray:@[@5, @6, @9, @8]];
    MCVector *c = [MCVector vectorWithValuesInArray:@[@1, @2, @3]];
    
    MCVector *quotient = [MCVector quotientOfVectorA:b andVectorB:a];
    MCVector *solution = [MCVector vectorWithValuesInArray:@[@5, @3, @3, @2]];
    for (int i = 0; i < 4; i++) {
        XCTAssertEqual([quotient valueAtIndex:i], [solution valueAtIndex:i], @"Value at index %u not divided correctly", i);
    }
    
    XCTAssertThrows([MCVector quotientOfVectorA:a andVectorB:c], @"Should throw a mismatched dimension exception");
}

- (void)testVectorCrossProduct
{
    MCVector *a = [MCVector vectorWithValuesInArray:@[@3, @(-3), @1]];
    MCVector *b = [MCVector vectorWithValuesInArray:@[@4, @9, @2]];
    MCVector *c = [MCVector crossProductOfVectorA:a andVectorB:b];
    MCVector *solution = [MCVector vectorWithValuesInArray:@[@(-15), @(-2), @39]];
    XCTAssertTrue([c isEqualToVector:solution], @"Cross product not computed correctly.");
}

- (void)testVectorCopying
{
    MCVector *a = [MCVector vectorWithValuesInArray:@[@3, @(-3), @1]];
    MCVector *aCopy = a.copy;
    
    XCTAssertNotEqual(a.self, aCopy.self, @"The copied vector is the same instance as its source.");
    XCTAssertTrue([a isEqualToVector:aCopy], @"Vector copy is not equal to its source.");
}

@end