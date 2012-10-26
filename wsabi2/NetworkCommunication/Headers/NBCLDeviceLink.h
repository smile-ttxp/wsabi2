//
//  NBCLSensorLink.h
//  Wsabi
//
//  Created by Matt Aronoff on 3/24/10.
//
/*
 This software was developed at the National Institute of Standards and Technology by employees of the Federal Government
 in the course of their official duties. Pursuant to title 17 Section 105 of the United States Code this software is not 
 subject to copyright protection and is in the public domain. Wsabi is an experimental system. NIST assumes no responsibility 
 whatsoever for its use by other parties, and makes no guarantees, expressed or implied, about its quality, reliability, or 
 any other characteristic. We would appreciate acknowledgement if the software is used.
 */

// Updated Feb 2012 for ARC support

#import <Foundation/Foundation.h>
#import "Base64Coder.h" //to decode the images
#import "NBCLXMLMap.h" //to convert from XML data to Objective-C objects
#import "WSModalityMap.h"
#import "WSBDResult.h"
#import "WSBDParameter.h"
#import "NBCLDeviceLinkConstants.h"
#import "constants.h"

#import "ASIHTTPRequest.h"
#import "ASINetworkQueue.h"

@class NBCLDeviceLink;

@protocol NBCLDeviceLinkDelegate <NSObject>

-(void) sensorOperationDidFail:(int)opType fromLink:(NBCLDeviceLink*)link sourceObjectID:(NSURL*)sourceID withError:(NSError*)error;
-(void) sensorOperationWasCancelledByService:(int)opType fromLink:(NBCLDeviceLink*)link sourceObjectID:(NSURL*)sourceID withResult:(WSBDResult*)result;
-(void) sensorOperationWasCancelledByClient:(int)opType fromLink:(NBCLDeviceLink*)link sourceObjectID:(NSURL*)sourceID;
-(void) sensorOperationCompleted:(int)opType fromLink:(NBCLDeviceLink*)link sourceObjectID:(NSURL*)sourceID withResult:(WSBDResult*)result;

-(void) sensorConnectionStatusChanged:(BOOL)connectedAndReady fromLink:(NBCLDeviceLink*)link sourceObjectID:(NSURL*)sourceID;

/** These are sequences of actions that we'll need to perform **/

//NOTE: The result object will be the result from the last performed step.
-(void) connectSequenceCompletedFromLink:(NBCLDeviceLink*)link 
                              withResult:(WSBDResult*)result 
                           sourceObjectID:(NSURL*)sourceID;

-(void) configureSequenceCompletedFromLink:(NBCLDeviceLink*)link 
                              withResult:(WSBDResult*)result 
                           sourceObjectID:(NSURL*)sourceID;

-(void) connectConfigureSequenceCompletedFromLink:(NBCLDeviceLink*)link 
                                withResult:(WSBDResult*)result 
                             sourceObjectID:(NSURL*)sourceID;

//The array of results in these sequences contains WSBDResults for each captureId.
//The tag is used to ID the UI element that made the request, so we can pass it the resulting data.
-(void) configCaptureDownloadSequenceCompletedFromLink:(NBCLDeviceLink*)link 
                                           withResults:(NSMutableArray*)results 
                                         sourceObjectID:(NSURL*)sourceID;

-(void) fullSequenceCompletedFromLink:(NBCLDeviceLink*)link 
                                           withResults:(NSMutableArray*)results 
                                         sourceObjectID:(NSURL*)sourceID;

-(void) disconnectSequenceCompletedFromLink:(NBCLDeviceLink*)link 
                                 withResult:(WSBDResult*)result 
                              sourceObjectID:(NSURL*)sourceID;

-(void) sequenceDidFail:(SensorSequenceType)sequenceType
                     fromLink:(NBCLDeviceLink*)link 
                   withResult:(WSBDResult*)result 
                sourceObjectID:(NSURL*)sourceID;

@end


@interface NBCLDeviceLink : NSObject <NSXMLParserDelegate> {

	//int maxImageSize;
	NSMutableArray *captureIds;
	
	SEL delegateSelector;
	
    //The configuration to be used in case we're running a capture sequence rather than
    //individual operations (in which case we need a place to put the config while we're progressing).
    NSMutableDictionary *pendingConfiguration;
    
	//The data being processed for the current request. 
	NSMutableData *responseData;
    //The data being processed for any pending cancel request.
    NSMutableData *cancelResponseData;

    int operationInProgress;
    int operationPendingCancellation;
    
    SensorSequenceType storedSequence;

	ASINetworkQueue *networkQueue;
    	
    NSTimeInterval networkTimeout;
	    
    NSMutableDictionary *downloadRetryCount;
    NSTimeInterval exponentialIntervalMax;
        
    //instance-only variables (no properties attached)
    BOOL shouldTryStealLock;
    float downloadMaxSize;
    int numCaptureIdsAwaitingDownload;
    	
}

+(NSString*) stringForOpType:(int)opType;

-(BOOL) checkHTTPStatus:(ASIHTTPRequest*)request;
-(void) attemptWSBDSequenceRecovery:(NSURL*)sourceObjectID; //try to figure out what problem we've got and re-establish the sequence.

#pragma mark - Convenience methods to combine multiple steps
-(BOOL) beginConnectSequenceWithSourceObjectID:(NSURL*)sourceID;

-(BOOL) beginConfigureSequence:(NSString*)sessionId
           configurationParams:(NSMutableDictionary*)params
                 sourceObjectID:(NSURL*)sourceID;

-(BOOL) beginConnectConfigureSequenceWithConfigurationParams:(NSMutableDictionary*)params
                 sourceObjectID:(NSURL*)sourceID;

-(BOOL) beginConfigCaptureDownloadSequence:(NSString*)sessionId
         configurationParams:(NSMutableDictionary*)params
                 withMaxSize:(float)maxSize 
               sourceObjectID:(NSURL*)sourceID;

-(BOOL) beginFullSequenceWithConfigurationParams:(NSMutableDictionary*)params
                               withMaxSize:(float)maxSize 
                             sourceObjectID:(NSURL*)sourceID;

-(BOOL) beginDisconnectSequence:(NSString*)sessionId sourceObjectID:(NSURL*)sourceID;


#pragma mark -
#pragma mark Async methods

//NOTE: The senderTag may be -1, in which case it is assumed that the caller isn't interested in tracking
//from where the call originated.

//Register
-(void) beginRegisterClient:(NSURL*)sourceObjectID;
-(void) beginUnregisterClient:(NSString*)sessionId sourceObjectID:(NSURL*)sourceID;

//Lock
-(void) beginLock:(NSString*)sessionId sourceObjectID:(NSURL*)sourceID;
-(void) beginLock:(NSString*)sessionId sourceObjectID:(NSURL*)sourceID;
-(void) beginStealLock:(NSString*)sessionId sourceObjectID:(NSURL*)sourceID;
-(void) beginUnlock:(NSString*)sessionId sourceObjectID:(NSURL*)sourceID;

//Info
-(void) beginGetServiceInfo:(NSURL*)sourceObjectID;

//Initialize
-(void) beginInitialize:(NSString*)sessionId sourceObjectID:(NSURL*)sourceID;

//Configure
-(void) beginGetConfiguration:(NSString*)sessionId sourceObjectID:(NSURL*)sourceID;
-(void) beginConfigure:(NSString*)sessionId withParameters:(NSDictionary*)params sourceObjectID:(NSURL*)sourceID;

//Capture
-(void) beginCapture:(NSString*)sessionId sourceObjectID:(NSURL*)sourceID;

//Download
-(void) beginDownload:(NSString*)captureId sourceObjectID:(NSURL*)sourceID;
-(void) beginDownload:(NSString*)captureId withMaxSize:(float)maxSize sourceObjectID:(NSURL*)sourceID;
-(void) beginGetDownloadInfo:(NSString*)captureId sourceObjectID:(NSURL*)sourceID;

//Cancel
-(void) beginCancel:(NSString*)sessionId sourceObjectID:(NSURL*)sourceID;

#pragma mark -
#pragma mark Async completion methods
-(void) sensorOperationFailed:(ASIHTTPRequest*)request;

//Register
-(void) registerClientCompleted:(ASIHTTPRequest*)request;
-(void) unregisterClientCompleted:(ASIHTTPRequest*)request;

//Lock
-(void) lockCompleted:(ASIHTTPRequest*)request;
-(void) stealLockCompleted:(ASIHTTPRequest*)request;
-(void) unlockCompleted:(ASIHTTPRequest*)request;

//Info
-(void) getServiceInfoCompleted:(ASIHTTPRequest*)request;

//Initialize
-(void) initializeCompleted:(ASIHTTPRequest*)request;

//Configure
-(void) getConfigurationCompleted:(ASIHTTPRequest*)request;
-(void) configureCompleted:(ASIHTTPRequest*)request;

//Capture
-(void) captureCompleted:(ASIHTTPRequest*)request;

//Download
//-(void) getDownloadInfoCompleted:(WSBDResult*)result;
-(void) downloadCompleted:(ASIHTTPRequest*)request; //this works for both beginDownload calls
-(void) getDownloadInfoCompleted:(ASIHTTPRequest*)request;

//-(void) downloadMostRecentCaptureCompleted:(WSBDResult*)result; //convenience method

//Cancel
-(void) cancelCompleted:(ASIHTTPRequest*)request;

#pragma mark -
#pragma mark Client-managed cancellation operations
-(void) cancelAllOperations;
-(void) cancelOperation:(ASIHTTPRequest*)request;

#pragma mark -
#pragma mark ASIHTTPRequest-related methods
-(BOOL) parseResultData:(NSMutableData*)parseableData;
- (void)cancelRequestStarted:(ASIHTTPRequest *)request;
- (void)cancelRequest:(ASIHTTPRequest *)request didReceiveData:(NSData *)data;
- (void) cancelRequestFailed:(ASIHTTPRequest*)request;


#pragma mark -
#pragma mark Properties
@property (nonatomic, strong) NSString *uri;
@property (nonatomic, strong) NSString *currentSessionId;
@property (nonatomic, strong) NSMutableArray *captureIds;
@property (nonatomic) NSTimeInterval networkTimeout;
@property (nonatomic) NSTimeInterval exponentialIntervalMax;

@property (nonatomic) BOOL connectedAndReady;

//WS-BD Variables
@property (nonatomic, strong) NSString *mainNamespace;
@property (nonatomic, strong) NSString *schemaInstanceNamespace;
@property (nonatomic, strong) NSString *schemaNamespace;

//IMPORTANT NOTE: Each of these is a GUIDELINE, and SUGGESTS that the sensor 
//is PROBABLY past the stated phase and connected. If the sensor is
//disconnected, or another client holds the lock when the next operation is
//performed, it will still fail.
@property (nonatomic) BOOL registered;
@property (nonatomic) BOOL hasLock;
@property (nonatomic) BOOL initialized;

@property (nonatomic) SensorSequenceType sequenceInProgress;
@property (nonatomic) BOOL shouldRetryDownloadIfPending;

//NSXMLParser variables
@property (nonatomic, strong) WSBDResult *currentWSBDResult;
@property (nonatomic, strong) WSBDParameter *currentWSBDParameter;
@property (nonatomic, strong) NSString *currentElementName;
@property (nonatomic, strong) NSString *currentElementValue;
@property (nonatomic, strong) NSDictionary *currentElementAttributes;
@property (nonatomic, strong) NSMutableArray *currentContainerArray;
@property (nonatomic, strong) NSMutableDictionary *currentContainerDictionary;
@property (nonatomic, strong) id currentDictionaryKey;
@property (nonatomic, strong) id currentDictionaryValue;
//@property (nonatomic) int maxImageSize;

@property (nonatomic, strong) NSDictionary *acceptableContentTypes;

@property (nonatomic, strong) NSMutableArray *downloadSequenceResults;

//Delegate
@property (nonatomic, unsafe_unretained) IBOutlet id<NBCLDeviceLinkDelegate> delegate;

@end