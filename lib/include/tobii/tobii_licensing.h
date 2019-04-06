/*
COPYRIGHT 2015 - PROPERTY OF TOBII AB
-------------------------------------
2015 TOBII AB - KARLSROVAGEN 2D, DANDERYD 182 53, SWEDEN - All Rights Reserved.

NOTICE:  All information contained herein is, and remains, the property of Tobii AB and its suppliers, if any.  
The intellectual and technical concepts contained herein are proprietary to Tobii AB and its suppliers and may be 
covered by U.S.and Foreign Patents, patent applications, and are protected by trade secret or copyright law. 
Dissemination of this information or reproduction of this material is strictly forbidden unless prior written 
permission is obtained from Tobii AB.
*/

#ifndef tobii_licensing_h_included
#define tobii_licensing_h_included

#include "tobii.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tobii_license_key_t
{
    uint16_t const* license_key;
    size_t size_in_bytes;
} tobii_license_key_t;

typedef enum tobii_license_validation_result_t
{
    TOBII_LICENSE_VALIDATION_RESULT_OK,
    TOBII_LICENSE_VALIDATION_RESULT_TAMPERED,
    TOBII_LICENSE_VALIDATION_RESULT_INVALID_APPLICATION_SIGNATURE,
    TOBII_LICENSE_VALIDATION_RESULT_NONSIGNED_APPLICATION,
    TOBII_LICENSE_VALIDATION_RESULT_EXPIRED,
    TOBII_LICENSE_VALIDATION_RESULT_PREMATURE,
    TOBII_LICENSE_VALIDATION_RESULT_INVALID_PROCESS_NAME,
    TOBII_LICENSE_VALIDATION_RESULT_INVALID_SERIAL_NUMBER,
    TOBII_LICENSE_VALIDATION_RESULT_INVALID_MODEL,
} tobii_license_validation_result_t;

TOBII_API tobii_error_t TOBII_CALL tobii_device_create_ex( tobii_api_t* api, char const* url,
    tobii_license_key_t const* license_keys, int license_count, tobii_license_validation_result_t* license_results,
    tobii_device_t** device );

TOBII_API tobii_error_t TOBII_CALL tobii_license_key_store( tobii_device_t* device,
    void* data, size_t size );
TOBII_API tobii_error_t TOBII_CALL tobii_license_key_retrieve( tobii_device_t* device,
    tobii_data_receiver_t receiver, void* user_data );

typedef enum tobii_feature_group_t
{
    TOBII_FEATURE_GROUP_BLOCKED,
    TOBII_FEATURE_GROUP_CONSUMER,
    TOBII_FEATURE_GROUP_CONFIG,
    TOBII_FEATURE_GROUP_PROFESSIONAL,
    TOBII_FEATURE_GROUP_INTERNAL,
} tobii_feature_group_t;


TOBII_API tobii_error_t TOBII_CALL tobii_get_feature_group( tobii_device_t* device,
    tobii_feature_group_t* feature_group );


#ifdef __cplusplus
}
#endif

#endif /* tobii_licensing_h_included */

/**
@defgroup tobii_licensing tobii_licensing.h

tobii_licensing.h
=================

This is the tobii_licensing.h file.
*/


/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_device_create_ex( tobii_api_t* api, char const* url, tobii_license_key_t const* license_keys, int license_count, tobii_license_validation_result_t* license_results, tobii_device_t** device );
@ingroup tobii_licensing

tobii_device_create_ex
----------------------

### Function

Creates a device instance to be used for communicating with a specific device with a certain license.


### Syntax

    #include <tobii/tobii.h>
    TOBII_API tobii_error_t TOBII_CALL tobii_device_create_ex( tobii_api_t* api, char const* url, tobii_license_key_t const* license_keys,
        int license_count, tobii_license_validation_result_t* license_results, tobii_device_t** device );


### Remarks

In order to communicate with a specific device, stream engine needs to keep track of a lot of internal state.
tobii_device_create_ex allocates and initializes this state, and is needed for all functions which communicates with a
device. Creating a device will establish a connection to the tracker, and can be used to query the device for more
information.

tobii_license_key_t is a basic structure that contains the license key and its size in bytes.

A license key is used for enabling extended functionality of the engine under certain conditions.
Conditions may include time limit, tracker model, tracker serial number, application name and/or application signature.
Every license key have one feature group which gives them a set of features. They may also include additional features
that are not included in their feature group. The device created will have all the features that provided by the valid
licences passed as argument. If there is no valid license, the feature group of the device will be consumer level.

Licenses are provided by Tobii AB.

*api* must be a pointer to a valid tobii_api_t as created by calling tobii_api_create.

*url* must be a valid device url as returned by tobii_enumerate_local_device_urls.

*license_keys* should be provided. It is an array of valid license keys provided by Tobii. At least one license must be
provided. Some API functions requires a different license than the basic consumer license:

*license_results* is optional. It is an array for returning the results of the license validation for each license. It
is adviced the check *license_results* in any case. All the error's is related with licensing will only return by this
array.

-    **Professional**
tobii_gaze_data_subscribe(),
tobii_gaze_data_unsubscribe(),
tobii_digital_syncport_subscribe()
tobii_digital_syncport_unsubscribe()
tobii_timesync()
tobii_set_illumination_mode()

-    **Config or Professional**
tobii_calibration_start()
tobii_calibration_stop()
tobii_calibration_collect_data_2d()
tobii_calibration_discard_data_2d()
tobii_calibration_clear()
tobii_calibration_compute_and_apply()
tobii_calibration_retrieve()
tobii_calibration_apply()
tobii_set_display_area()
tobii_set_output_frequency()
tobii_set_device_name()

-    **Additional Features**
tobii_image_subscribe()

*count* must be provided. It is the number of license keys has provided.

*device* must be a pointer to a variable of the type `tobii_device_t*` that is, a pointer to a tobii_device_t-pointer.
This variable will be filled in with a pointer to the created device. tobii_device_t is an opaque type, and only its
declaration is available in the API, it's definition is internal.


### Return value

If the device is successfully created, tobii_device_create returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_device_create returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *api*, *url*, *device* or *license_keys* parameters were passed in as NULL, or the *count* parameter was not
    non-zero.

-   **TOBII_ERROR_ALLOCATION_FAILED**

    The internal call to malloc or to the custom memory allocator (if used) returned NULL, so device creation failed.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve(), tobii_enumerate_illumination_modes(), 
    or tobii_license_key_retrieve().
    Calling tobii_device_create_ex from within a callback function is not supported.

### License Errors

-   TOBII_LICENSE_VALIDATION_RESULT_OK

The license that has been provided is valid.

-   TOBII_LICENSE_VALIDATION_RESULT_TAMPERED

The license file has been tampered.

-   TOBII_LICENSE_VALIDATION_RESULT_INVALID_APPLICATION_SIGNATURE

The signature of the application that runs the stream engine is not same with the signature in the license file.

-   TOBII_LICENSE_VALIDATION_RESULT_NONSIGNED_APPLICATION

The application that runs the stream engine has not been signed.

-   TOBII_LICENSE_VALIDATION_RESULT_EXPIRED

The validity of the license has been expired.

-   TOBII_LICENSE_VALIDATION_RESULT_PREMATURE

The license is not valid yet.

-   TOBII_LICENSE_VALIDATION_RESULT_INVALID_PROCESS_NAME

The process name of the application that runs the stream engine is not included to the list of process names in the
license file.

-   TOBII_LICENSE_VALIDATION_RESULT_INVALID_SERIAL_NUMBER

The serial number of the current eye tracker is not included to the list of serial numbers in the license file.

-   TOBII_LICENSE_VALIDATION_RESULT_INVALID_MODEL

The model name of the current eye tracker is not included to the list of model names in the license file.

### See also

tobii_device_destroy(), tobii_enumerate_local_device_urls(), tobii_api_create(), tobii_get_device_info(),
tobii_get_feature_group() tobii_device_create()


### Example
@code{.c}

    #include "tobii/tobii.h"
    #include "tobii/tobii_licensing.h"

    #include <stdio.h>
    #include <malloc.h>
    #include <memory.h>
    #include <assert.h>

    static size_t read_license_file( uint16_t* license )
    {
        FILE *license_file = fopen( "se_license_key_sample", "rb" );

        if( !license_file )
        {
            printf( "License key could not be found!" );
            return 0;
        }

        fseek( license_file, 0, SEEK_END );
        long file_size = ftell( license_file );
        rewind( license_file );

        if( file_size <= 0 )
        {
            printf( "License file is empty!" );
            return 0;
        }

        if( license )
        {
            fread( license, sizeof( uint16_t ), file_size / sizeof( uint16_t ), license_file );
        }

        fclose( license_file );
        return ( size_t )file_size;
    }

    static void url_receiver( char const* url, void* user_data )
    {
        char* buffer = (char*)user_data;
        if( *buffer != '\0' ) return; // only keep first value

        if( strlen( url ) < 256 )
            strcpy( buffer, url );
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        size_t license_size = read_license_file( 0 );
        assert( license_size > 0 );

        uint16_t* license_key = ( uint16_t* )malloc( license_size );
        memset( license_key, 0, license_size );
        read_license_file( license_key );

        tobii_license_key_t license = { license_key, license_size };
        tobii_license_validation_result_t validation_result;

        char url[ 256 ] = { 0 };
        error = tobii_enumerate_local_device_urls( api, url_receiver, url );
        assert( error == TOBII_ERROR_NO_ERROR && *url != '\0' );

        tobii_device_t* device;
        error = tobii_device_create_ex( api, url, &license, 1, &validation_result, &device );
        free( license_key );
        assert( error == TOBII_ERROR_NO_ERROR );

        // --> code to use the device would go here <--

        error = tobii_device_destroy( device );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_api_destroy( api );
        assert( error == TOBII_ERROR_NO_ERROR );
        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_license_key_store( tobii_device_t* device, void* data, size_t size );
@ingroup tobii_licensing

tobii_license_key_store
-----------------------

### Function

Stores the license key on the tracker


### Syntax

    #include <tobii/tobii.h>
    tobii_error_t tobii_license_key_store( tobii_device_t* device, void* data, size_t size );


### Remarks

license key can be stored on the device. Only one key will be stored on the device so calling the API will overwrite
the old key. If either data or size is passed as 0 then it will erase the already stored license key.


*device* must be a pointer to a variable of the type `tobii_device_t*` that is, a pointer to a tobii_device_t.

*data* has to be in uint16_t text passed as the void*. It is optional and hence if it is 0 then it will erase already
stored license

*size* is the no of bytes in the data buffer. If it is passed as 0 then it will erase already stored license.


### Return value

If the device is successfully created, tobii_device_create returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_device_create returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_ALLOCATION_FAILED**

    The internal call to malloc or to the custom memory allocator (if used) returned NULL, so device creation failed.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The device doesn't support storage APIs. This error is returned if the API is called with an old device which
    doesn't support the license device store.

-   **TOBII_ERROR_OPERATION_FAILED**

    Writting to the the device failed because of unexpected IO error, file not found, storage is full or filename is
    invalid.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve(), tobii_enumerate_illumination_modes(), 
    or tobii_license_key_retrieve().
    Calling tobii_license_key_store from within a callback function is not supported.

### See also

tobii_license_key_retrieve(), tobii_device_create()


### Example
@code{.c}

    #include "tobii/tobii_licensing.h"

    #include <stdio.h>
    #include <malloc.h>
    #include <memory.h>
    #include <assert.h>

    static size_t read_license_file( uint16_t* license )
    {
        FILE *license_file = fopen( "se_license_key_sample", "rb" );

        if( !license_file )
        {
            printf( "License key could not be found!" );
            return 0;
        }

        fseek( license_file, 0, SEEK_END );
        long file_size = ftell( license_file );
        rewind( license_file );

        if( file_size <= 0 )
        {
            printf( "License file is empty!" );
            return 0;
        }

        if( license )
        {
            fread( license, sizeof( uint16_t ), file_size / sizeof( uint16_t ), license_file );
        }

        fclose( license_file );
        return ( size_t )file_size;
    }

    void data_receiver( void const* data, size_t size, void* user_data )
    {
        if ( !data || !size || !user_data ) return;   // user_data shouldn't be NULL if passed as Non NULL

        // The license is received here,
        // --> code to use the device would go here <--
        //  We will just compare if the store was ok for demo pupose.
        tobii_license_key_t* license = ( tobii_license_key_t* )user_data;
        if( size != license->size_in_bytes ) return;
        if( !memcmp( (void*)license->license_key, data, size ) )
            printf("Data Received correctly");
        else
            printf( "Invalid Data Received" );
    }

    static void url_receiver( char const* url, void* user_data )
    {
        char* buffer = (char*)user_data;
        if( *buffer != '\0' ) return; // only keep first value

        if( strlen( url ) < 256 )
            strcpy( buffer, url );
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        size_t license_size = read_license_file( 0 );
        assert( license_size > 0 );

        uint16_t* license_key = ( uint16_t* )malloc( license_size );
        memset( license_key, 0, license_size );
        read_license_file( license_key );

        tobii_license_key_t license = { license_key, license_size };
        tobii_license_validation_result_t validation_result;

        char url[ 256 ] = { 0 };
        error = tobii_enumerate_local_device_urls( api, url_receiver, url );
        assert( error == TOBII_ERROR_NO_ERROR && *url != '\0' );

        tobii_device_t* device;
        error = tobii_device_create_ex( api, url, &license, 1, &validation_result, &device );
        if ( error != TOBII_ERROR_NO_ERROR ) free( license_key );
        assert( error == TOBII_ERROR_NO_ERROR );

        // Store The license to the device
        error = tobii_license_key_store( device, (void*) license.license_key,
            license.size_in_bytes );
        if( error != TOBII_ERROR_NO_ERROR ) free( license_key );
        assert( error == TOBII_ERROR_NO_ERROR );

        // Retrieve the license from the device
        error = tobii_license_key_retrieve( device, data_receiver, (void*)&license );
        free( license_key );
        assert( error == TOBII_ERROR_NO_ERROR );

        // Erase the license from the device
        error = tobii_license_key_store( device, 0, 0 );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_device_destroy( device );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_api_destroy( api );
        assert( error == TOBII_ERROR_NO_ERROR );
        return 0;
    }


@endcode

*/


/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_license_key_retrieve( tobii_device_t* device, tobii_data_receiver_t receiver, void* user_data );
@ingroup tobii_licensing

tobii_license_key_retrieve
--------------------------

### Function

Retreives the already stored license key from the device.


### Syntax

    #include <tobii/tobii.h>
    tobii_error_t tobii_license_key_retrieve( tobii_device_t* device, tobii_data_receiver_t receiver, void* user_data );


### Remarks

The receiver function passed as the parameter receives the data.
Instead of storing the pointer to data, content of the data should be copied
as the data pointer becomes invalid after the call is over.

*device* must be a pointer to a variable of the type `tobii_device_t*` that is, a pointer to a tobii_device_t-pointer.
the device is obtained by calling tobii_device_create() or by tobii_device_create_ex(). It must be freed by
calling tobii_device_destroy() as clean up operation.

*receiver* is a function pointer to a function with the prototype:

    void data_receiver( void const* data, size_t size, void* user_data )

This function will be called with the retrieved license data. It is called with the following parameters:

-   *data*
    The license data read from device. This pointer will be invalid after returning from the function,
    so ensure you make a copy of the data rather than storing the pointer directly.

-   *size*
    This gives the size of the data buffer read.

-   *user_data*
    This is the custom pointer sent in to tobii_license_key_retrieve.

*user_data* is optional. Caller can pass any data here as the calling device which could be used in the *receiver*.


### Return value

If the device is successfully created, tobii_device_create returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_device_create returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_ALLOCATION_FAILED**

    The internal call to malloc or to the custom memory allocator (if used) returned NULL, so device creation failed.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The device doesn't support storage APIs. This error is returned if the API is called with an old device which
    doesn't support the license device store.

-   **TOBII_ERROR_OPERATION_FAILED**

    Reading from the device failed because of unexpected IO error, file not found, filename is invalid.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve(), tobii_enumerate_illumination_modes(), 
    or tobii_license_key_retrieve().
    Calling tobii_license_key_retrieve from within a callback function is not supported.

### See also

tobii_license_key_retrieve(), tobii_device_create()


### Example
@code{.c}

    #include "tobii/tobii_licensing.h"

    #include <stdio.h>
    #include <malloc.h>
    #include <memory.h>
    #include <assert.h>

    static size_t read_license_file( uint16_t* license )
    {
        FILE *license_file = fopen( "se_license_key_sample", "rb" );

        if( !license_file )
        {
            printf( "License key could not be found!" );
            return 0;
        }

        fseek( license_file, 0, SEEK_END );
        long file_size = ftell( license_file );
        rewind( license_file );

        if( file_size <= 0 )
        {
            printf( "License file is empty!" );
            return 0;
        }

        if( license )
        {
            fread( license, sizeof( uint16_t ), file_size / sizeof( uint16_t ), license_file );
        }

        fclose( license_file );
        return ( size_t )file_size;
    }

    void data_receiver( void const* data, size_t size, void* user_data )
    {
        if ( !data || !size || !user_data ) return;   // user_data shouldn't be NULL if passed as Non NULL

        // The license is received here,
        // --> code to use the device would go here <--
        //  We will just compare if the store was ok for demo pupose.
        tobii_license_key_t* license = ( tobii_license_key_t* )user_data;
        if( size != license->size_in_bytes ) return;
        if( !memcmp( (void*)license->license_key, data, size ) )
            printf("Data Received correctly");
        else
            printf( "Invalid Data Received" );
    }

    static void url_receiver( char const* url, void* user_data )
    {
        char* buffer = (char*)user_data;
        if( *buffer != '\0' ) return; // only keep first value

        if( strlen( url ) < 256 )
            strcpy( buffer, url );
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        size_t license_size = read_license_file( 0 );
        assert( license_size > 0 );

        uint16_t* license_key = ( uint16_t* )malloc( license_size );
        memset( license_key, 0, license_size );
        read_license_file( license_key );

        tobii_license_key_t license = { license_key, license_size };
        tobii_license_validation_result_t validation_result;

        char url[ 256 ] = { 0 };
        error = tobii_enumerate_local_device_urls( api, url_receiver, url );
        assert( error == TOBII_ERROR_NO_ERROR && *url != '\0' );

        tobii_device_t* device;
        error = tobii_device_create_ex( api, url, &license, 1, &validation_result, &device );
        if ( error != TOBII_ERROR_NO_ERROR ) free( license_key );
        assert( error == TOBII_ERROR_NO_ERROR );

        // Store The license to the device
        error = tobii_license_key_store( device, (void*) license.license_key,
            license.size_in_bytes );
        if( error != TOBII_ERROR_NO_ERROR ) free( license_key );
        assert( error == TOBII_ERROR_NO_ERROR );

        // Retrieve the license from the device
        error = tobii_license_key_retrieve( device, data_receiver, (void*)&license );
        free( license_key );
        assert( error == TOBII_ERROR_NO_ERROR );

        // Erase the license from the device
        error = tobii_license_key_store( device, 0, 0 );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_device_destroy( device );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_api_destroy( api );
        assert( error == TOBII_ERROR_NO_ERROR );
        return 0;
    }
@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_get_feature_group( tobii_device_t* device, tobii_feature_group_t* feature_group );
@ingroup tobii_licensing

tobii_get_feature_group
-----------------------

### Function

Retrieves the currently active feature group for a device.


### Syntax

    #include <tobii/tobii_advanced.h>
    tobii_error_t tobii_get_feature_group( tobii_device_t* device, tobii_feature_group_t* feature_group );


### Remarks

The currently active feature group is determined by tobii_device_create based on the license key passed into it.
tobii_get_feature_group can be used to query the currently active feature group.

*device* must be a pointer to a valid tobii_device_t as created by calling tobii_device_create.

*feature_group* is a pointer to a tobii_feature_group_t to receive the current group, in the form of values from the
following enum:

-   **TOBII_FEATURE_GROUP_BLOCKED**

    The provided license key was invalid, or the application making the call has been blacklisted. No API functionality
    will be available.

-   **TOBII_FEATURE_GROUP_CONSUMER**

    Default feature group for passing a NULL (default) license key to tobii_device_create. Gives access to all API
    functions except those where a higher feature group is specified in the documentation.

-   **TOBII_FEATURE_GROUP_CONFIG**

    Grants access to functionality that changes configuration of the tracker (mainly in tobii_config.h). This feature
    group might be automatically granted for certain devices, like head-mounted displays, even if a default license key
    is used.

-   **TOBII_FEATURE_GROUP_PROFESSIONAL**

    Gives access to the functionality in tobii_advanced.h. This feature group might be automatically granted for
    professional level devices, as supplied by Tobii Pro, even if a default license key is used.

-   **TOBII_FEATURE_GROUP_INTERNAL**

    For internal use by Tobii.


The current feature group controls which API features are available. The documentation will state which functions
require a specific license (if it is not specified, it is assumed that **TOBII_FEATURE_GROUP_CONSUMER** is required).

Each feature group includes all feature groups preceding it (with the exception of **TOBII_FEATURE_GROUP_BLOCKED**,
which indicates that the specified license key was found to be invalid, or the current application has been blacklisted,
in which case no API functions will be available).


### Return value

If the feature group was successfully retrieved, tobii_get_feature_group returns **TOBII_ERROR_NO_ERROR**. If the call
fails, tobii_get_feature_group returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    One or more of the *device* and *feature_group* parameters were passed in as NULL. *device* must be a valid
    tobii_device_t pointer as created by tobii_device_create, and *feature_group* must be a valid pointer to a
    tobii_feature_group_t variable.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve(), tobii_enumerate_illumination_modes(), 
    or tobii_license_key_retrieve().
    Calling tobii_get_feature_group from within a callback function is not supported.

### See also

tobii_device_create()


### Example
@code{.c}

    #include <tobii/tobii_licensing.h>
    #include <stdio.h>
    #include <assert.h>

    static void url_receiver( char const* url, void* user_data )
    {
        char* buffer = (char*)user_data;
        if( *buffer != '\0' ) return; // only keep first value

        if( strlen( url ) < 256 )
            strcpy( buffer, url );
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        char url[ 256 ] = { 0 };
        error = tobii_enumerate_local_device_urls( api, url_receiver, url );
        assert( error == TOBII_ERROR_NO_ERROR && *url != '\0' );

        tobii_device_t* device;
        error = tobii_device_create( api, url, &device );
        assert( error == TOBII_ERROR_NO_ERROR );

        tobii_feature_group_t feature_group;
        error = tobii_get_feature_group( device, &feature_group );
        assert( error == TOBII_ERROR_NO_ERROR );

        if( feature_group == TOBII_FEATURE_GROUP_CONSUMER )
            printf( "Running with 'consumer' feature group.\n" );

        error = tobii_device_destroy( device );
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_api_destroy( api );
        assert( error == TOBII_ERROR_NO_ERROR );
        return 0;
    }

@endcode

*/

