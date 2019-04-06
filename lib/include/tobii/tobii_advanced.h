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

#ifndef tobii_advanced_h_included
#define tobii_advanced_h_included

#include "tobii.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tobii_gaze_data_eye_t
{
    tobii_validity_t gaze_origin_validity;
    float gaze_origin_from_eye_tracker_mm_xyz[ 3 ];
    float eye_position_in_track_box_normalized_xyz[ 3 ];

    tobii_validity_t gaze_point_validity;
    float gaze_point_from_eye_tracker_mm_xyz[ 3 ];
    float gaze_point_on_display_normalized_xy[ 2 ];

    tobii_validity_t eyeball_center_validity;
    float eyeball_center_from_eye_tracker_mm_xyz[ 3 ];

    tobii_validity_t pupil_validity;
    float pupil_diameter_mm;
} tobii_gaze_data_eye_t;

typedef struct tobii_gaze_data_t
{
    int64_t timestamp_tracker_us;
    int64_t timestamp_system_us;
    tobii_gaze_data_eye_t left;
    tobii_gaze_data_eye_t right;
} tobii_gaze_data_t;

typedef void (*tobii_gaze_data_callback_t)( tobii_gaze_data_t const* gaze_data, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_gaze_data_subscribe( tobii_device_t* device,
    tobii_gaze_data_callback_t callback, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_gaze_data_unsubscribe( tobii_device_t* device );

typedef void (*tobii_digital_syncport_callback_t)( uint32_t signal, int64_t timestamp_tracker_us,
    int64_t timestamp_system_us, void* user_data );

TOBII_API tobii_error_t TOBII_CALL tobii_digital_syncport_subscribe( tobii_device_t* device,
    tobii_digital_syncport_callback_t callback, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_digital_syncport_unsubscribe( tobii_device_t* device );

typedef struct tobii_timesync_data_t
{
    int64_t system_start_us;
    int64_t system_end_us;
    int64_t tracker_us;
} tobii_timesync_data_t;

TOBII_API tobii_error_t TOBII_CALL tobii_timesync( tobii_device_t* device,
    tobii_timesync_data_t* timesync );


typedef char tobii_illumination_mode_t[ 64 ];
typedef void (*tobii_illumination_mode_receiver_t)( const tobii_illumination_mode_t illumination_mode,
    void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_enumerate_illumination_modes( tobii_device_t* device,
    tobii_illumination_mode_receiver_t receiver, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_set_illumination_mode( tobii_device_t* device,
    tobii_illumination_mode_t const illumination_mode );
TOBII_API tobii_error_t TOBII_CALL tobii_get_illumination_mode( tobii_device_t* device,
    tobii_illumination_mode_t* illumination_mode );

typedef char tobii_face_type_t[ 64 ];
typedef void (*tobii_face_type_receiver_t)( const tobii_face_type_t face_type,
    void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_enumerate_face_types( tobii_device_t* device,
    tobii_face_type_receiver_t receiver, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_set_face_type( tobii_device_t* device,
    tobii_face_type_t const face_type );
TOBII_API tobii_error_t TOBII_CALL tobii_get_face_type( tobii_device_t* device,
    tobii_face_type_t* face_type );



#ifdef __cplusplus
}
#endif

#endif /* tobii_advanced_h_included */

/**
@defgroup tobii_advanced tobii_advanced.h

tobii_advanced.h
=======

This is the tobii_advanced.h file.
*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_gaze_data_subscribe( tobii_device_t* device, tobii_gaze_data_callback_t callback, void* user_data );
@ingroup tobii_advanced

tobii_gaze_data_subscribe
-------------------------

### Function

Starts the gaze data stream.


### Syntax

    #include <tobii/tobii_advanced.h>
    tobii_error_t tobii_gaze_data_subscribe( tobii_device_t* device,
        tobii_gaze_data_callback_t callback, void* user_data );


### Remarks

To be able to call this function, the *device* should have been created with a minimum license level of Professional
feature group.

*device* must be a pointer to a valid tobii_device_t as created by calling tobii_device_create.

*callback* is a function pointer to a function with the prototype:

    void gaze_data_callback( tobii_gaze_data_t const* gaze_data, void* user_data )

Older devices using the deprecated 0-4 scale to determine validity will have the value map to the new binary scale accordingly:

    0 - TOBII_VALIDITY_VALID
    1 - TOBII_VALIDITY_VALID
    2 - TOBII_VALIDITY_INVALID
    3 - TOBII_VALIDITY_INVALID
    4 - TOBII_VALIDITY_INVALID

This function will be called when there is new gaze data available. It is called with the following parameters:

-   *gaze_data*
This is a pointer to a struct containing the data listed below. Note that it is only valid during the callback. Its data
should be copied if access is necessary at a later stage, from outside the callback.

    -	*timestamp_tracker_us*
    Timestamp value for when the gaze data was captured in microseconds (us). It is generated on the device responsible for capturing the data.
    *timestamp_system_us* is generated using this value. The epoch is undefined, so these timestamps are only useful for calculating
    the time elapsed between a pair of values.

    -   *timestamp_system_us*
    Timestamp value for when the gaze data was captured, measured in microseconds (us). The epoch is undefined, 
    so these timestamps are only useful for calculating the time elapsed between a pair of values. The function tobii_system_clock
    can be used to retrieve a timestamp using the same clock and same relative values as this timestamp.

    -   *left*
    This is a struct containing the following data, related to the left eye:

        -   *gaze_origin_validity*
        **TOBII_VALIDITY_INVALID** if *gaze_origin_mm_xyz* and *gaze_origin_in_track_box_normalized* are not valid for
        this frame, **TOBII_VALIDITY_VALID** if they are.

        -   *gaze_origin_from_eye_tracker_mm*
        An array of three floats, for the x, y and z coordinate of the gaze origin point of the eye of the user, as
        measured in millimeters from the center of the device.

        -   *gaze_origin_in_track_box_normalized*
        An array of three floats, for the x, y and z coordinate of the gaze origin point of the eye of the user, as 
        measured in the normalized distance of the device track box.

        -   *gaze_point_validity*
        **TOBII_VALIDITY_INVALID** if *gaze_point_from_eye_tracker_mm* and *gaze_point_on_display_normalized* 
        are not valid for this frame, **TOBII_VALIDITY_VALID** if they are.

        -   *gaze_point_from_eye_tracker_mm*
        An array of three floats, for the x, y and z coordinate of the gaze point that the user is currently looking,
        as measured in millimeters from the center of the device.

        -   *gaze_point_on_display_normalized*
        The horizontal and vertical screen coordinate of the gaze point. The left edge of the screen is 0.0, and the
        right edge is 1.0. The top edge of the screen is 0.0, and the bottom edge is 1.0.
        Note that the value might be outside the 0.0 to 1.0 range, if the user looks outside the screen.

        -   *eyeball_center_validity*
        **TOBII_VALIDITY_INVALID** if *eyeball_center_from_eye_tracker_mm* is not valid for this frame,
        **TOBII_VALIDITY_VALID** if it is.

        -   *eyeball_center_from_eye_tracker_mm*
        An array of three floats, for the x, y and z coordinate of the center of the eyeball, as
        measured in millimeters from the center of the device.

        -   *pupil_validity*
        **TOBII_VALIDITY_INVALID** if *pupil_diameter_mm* is not valid for this frame, **TOBII_VALIDITY_VALID** if it is.

        -   *pupil_diameter_mm*
        A float that represents the approximate diameter of the pupil, expressed in millimeters. Only relative changes
        are guaranteed to be accurate.

    -   *right*
    This is another instance of the same struct as in *left*, but which holds data related to the right eye of the user.

-   *user_data*
This is the custom pointer sent in when registering the callback.

*user_data* custom pointer which will be passed unmodified to the callback.


### Return value

If the call was successful **TOBII_ERROR_NO_ERROR** will be returned. If the call fails, tobii_gaze_data_subscribe 
returns an error code specific to the device.

### See Also

tobii_gaze_data_unsubscribe()

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_gaze_data_unsubscribe( tobii_device_t* device );
@ingroup tobii_advanced

tobii_gaze_data_unsubscribe
---------------------------

### Function

Stops the gaze data stream.


### Syntax

    #include <tobii/tobii_advanced.h>
    tobii_gaze_data_unsubscribe( tobii_device_t* device );


### Remarks

To be able to call this function, the *device* should have been created with a minimum license level of Professional
feature group. *device* must be a pointer to a valid tobii_device_t as created by calling tobii_device_create.


### Return value

If the call was successful **TOBII_ERROR_NO_ERROR** will be returned. If the call fails, tobii_gaze_data_unsubscribe
returns an error code specific to the device.

### See Also

tobii_gaze_data_subscribe()


### Example

@code{.c}

    #include "tobii/tobii.h"
    #include "tobii/tobii_licensing.h"
    #include "tobii/tobii_advanced.h"

    #include <stdio.h>
    #include <assert.h>

    static void tobii_gaze_data_callback(  tobii_gaze_data_t const* gaze_data, void* user_data  )
    {
        (void)user_data;
        if( gaze_data->right.gaze_point_validity == TOBII_VALIDITY_VALID )
            printf( "Gaze point (right): %f, %f\n", 
            gaze_data->right.gaze_point_on_display_normalized_xy[ 0 ], 
            gaze_data->right.gaze_point_on_display_normalized_xy[ 1 ] );
        else
            printf( "Gaze point (right): INVALID\n");

        if( gaze_data->left.gaze_point_validity == TOBII_VALIDITY_VALID )
            printf( "Gaze point (left): %f, %f\n", 
            gaze_data->left.gaze_point_on_display_normalized_xy[ 0 ], 
            gaze_data->left.gaze_point_on_display_normalized_xy[ 1 ] );
        else
            printf( "Gaze point (left): INVALID\n");
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        tobii_device_t* device;
        char url[ 256 ] = { 0 };
        printf( "Enter url to the eye tracker (don't forget prefix tobii-ttp:// or tet-tcp://):\n" );
        scanf( "%255s", url );
        error = tobii_device_create( api, url, &device );      // if not using a pro tracker use tobii_device_create_ex with Professional license
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_gaze_data_subscribe( device, tobii_gaze_data_callback, 0 );
        assert( error == TOBII_ERROR_NO_ERROR );

        int is_running = 10; // in this sample, exit after some iterations
        while( --is_running > 0 )
        {
            error = tobii_wait_for_callbacks( NULL, 1, &device );
            assert( error == TOBII_ERROR_NO_ERROR || error == TOBII_ERROR_TIMED_OUT );

            error = tobii_device_process_callbacks( device );
            assert( error == TOBII_ERROR_NO_ERROR );
        }

        error = tobii_gaze_data_unsubscribe( device );
        assert( error == TOBII_ERROR_NO_ERROR );

        tobii_device_destroy( device );
        tobii_api_destroy( api );

        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_digital_syncport_subscribe( tobii_device_t* device, tobii_digital_syncport_callback_t callback, void* user_data );
@ingroup tobii_advanced

tobii_digital_syncport_subscribe
--------------------------------

### Function

The digital syncport data stream subscription provides a sparse stream of the device's external port data in sync with
the device clock. This stream will provide new data when the syncport data value changes. Each change on the port is
timestamped with the same clock as the gaze data.


### Syntax

    #include <tobii/tobii_advanced.h>
    tobii_error_t tobii_digital_syncport_subscribe( tobii_device_t* device,
        tobii_digital_syncport_callback_t callback, void* user_data );


### Remarks

To be able to call this function, the *device* should have been created with a minimum license level of Professional
feature group.

*device* must be a pointer to a valid tobii_device_t as created by calling tobii_device_create.

*callback* is a function pointer to a function with the prototype:

    void digital_syncport_callback( uint32_t signal, int64_t timestamp_tracker_us,
        int64_t timestamp_system_us, void* user_data )

    This function will be called when the syncport data value changes. It is called with the following parameters:

    -   *signal*
    An unsigned integer from the external port. In the Spectrum device, only 8 bits are valid. Please
    check the hardware documentation of the relevant device for its valid bits.

    -	*timestamp_tracker_us*
    Timestamp value for when the digital syncport data was captured in microseconds (us). It is generated on the
    device responsible for capturing the data. *timestamp_system_us* is generated using this value.
    The epoch is undefined, so these timestamps are only useful for calculating the time elapsed between a pair of values.

    -   *timestamp_system_us*
    Timestamp value for when the digital syncport data was captured, measured in microseconds (us). The epoch is undefined,
    so these timestamps are only useful for calculating the time elapsed between a pair of values. The function
    tobii_system_clock can be used to retrieve a timestamp using the same clock and same relative values as this
    timestamp.

    -   *user_data* the custom pointer sent in when registering the callback.

*user_data* custom pointer which will be passed unmodified to the callback.


### Return value

If the call was successful **TOBII_ERROR_NO_ERROR** will be returned. If the call has failed one of the following error
will be returned:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter has been passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not valid, or has been blacklisted.

-   **TOBII_ERROR_ALREADY_SUBSCRIBED**

    A subscription for digital syncport data was already made. There can only be one callback registered at a time.
    To change to another callback, first call tobii_digital_syncport_unsubscribe().

-   **TOBII_ERROR_TOO_MANY_SUBSCRIBERS**

    Too many subscribers for the requested stream. Tobii eye trackers can have a limitation on the number of concurrent
    subscribers to specific streams due to high bandwidth and/or high frequency of the data stream.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve(), tobii_enumerate_illumination_modes(), 
    or tobii_license_key_retrieve().
    Calling tobii_digital_syncport_subscribe from within a callback function is not supported.

### See Also

tobii_digital_syncport_unsubscribe()

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_digital_syncport_unsubscribe( tobii_device_t* device );
@ingroup tobii_advanced

tobii_digital_syncport_unsubscribe
----------------------------------

### Function

Stops the digital syncport data stream.


### Syntax

    #include <tobii/tobii_advanced.h>
    tobii_digital_syncport_unsubscribe( tobii_device_t* device );


### Remarks

To be able to call this function, the *device* should have been created with a minimum license level of Professional
feature group. *device* must be a pointer to a valid tobii_device_t as created by calling tobii_device_create.


### Return value

If the call was successful **TOBII_ERROR_NO_ERROR** will be returned. If the call has failed one of the following error
will be returned:

-   **TOBII_ERROR_INVALID_PARAMETER**

The *device* parameter has been passed in as NULL.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

The provided license was not valid, or has been blacklisted.

-   **TOBII_ERROR_NOT_SUBSCRIBED**

A subscription for digital syncport data was not made before the call to unsubscribe.

-   **TOBII_ERROR_INTERNAL**

Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
the support

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve(), tobii_enumerate_illumination_modes(), 
    or tobii_license_key_retrieve().
    Calling tobii_digital_syncport_unsubscribe from within a callback function is not supported.

### See Also

tobii_digital_syncport_subscribe()


### Example

@code{.c}

    #include "tobii/tobii.h"
    #include "tobii/tobii_licensing.h"
    #include "tobii/tobii_advanced.h"

    #include <stdio.h>
    #include <assert.h>

    static void tobii_digital_syncport_callback( uint32_t signal, int64_t timestamp_tracker_us,
    int64_t timestamp_system_us, void* user_data )
    {
        (void)timestamp_tracker_us;(void)timestamp_system_us;(void)user_data;
        printf( "Digital syncport data is %d .\n", signal & 0xff ); // only 8 bits are valid for spectrum tacker
    }

    int main()
    {
        tobii_api_t* api;
        tobii_error_t error = tobii_api_create( &api, NULL, NULL );
        assert( error == TOBII_ERROR_NO_ERROR );

        tobii_device_t* device;
        char url[ 256 ] = { 0 };
        printf( "Enter url to the eye tracker (don't forget prefix tobii-ttp:// or tet-tcp://):\n" );
        scanf( "%255s", url );
        error = tobii_device_create( api, url, &device );      // if not using a pro tracker use tobii_device_create_ex with Professional license
        assert( error == TOBII_ERROR_NO_ERROR );

        error = tobii_digital_syncport_subscribe( device, tobii_digital_syncport_callback, 0 );
        assert( error == TOBII_ERROR_NO_ERROR );

        int is_running = 10; // in this sample, exit after some iterations
        while( --is_running > 0 )
        {
            error = tobii_wait_for_callbacks( NULL, 1, &device );
            assert( error == TOBII_ERROR_NO_ERROR || error == TOBII_ERROR_TIMED_OUT );

            error = tobii_device_process_callbacks( device );
            assert( error == TOBII_ERROR_NO_ERROR );
        }

        error = tobii_digital_syncport_unsubscribe( device );
        assert( error == TOBII_ERROR_NO_ERROR );

        tobii_device_destroy( device );
        tobii_api_destroy( api );

        return 0;
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_enumerate_face_types( tobii_device_t* device, tobii_face_type_receiver_t receiver, void* user_data )
@ingroup tobii_advanced

tobii_enumerate_face_types
----------------------

### Function

Retreives all supported face types from a specific eye tracker.


### Syntax

    #include <tobii/tobii_advanced.h>
    tobii_error_t tobii_enumerate_face_types( tobii_device_t* device, tobii_face_type_receiver_t receiver,
      void* user_data );


### Remarks

A face type is here understood as a class of appearances of the face itself, such as a group of species (e.g. human or
crocodile), facial features (e.g. moustache or makeup), or worn objects (e.g. glasses or hats). It is only used for
situations were auto detecting such differences is difficult or dangerous.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*receiver* is a function pointer to a function with the prototype:

void face_type_receiver( const tobii_face_type_t face_type, void* user_data );

This function will be called for each face type found during enumeration. It is called with the following parameters:

-   *face_type*
    A zero terminated string representation of a face type, max 63 characters long. This pointer will be invalid after
    returning from the function, so ensure you make a copy of the string rather than storing the pointer directly.

-   *user_data*
    This is the custom pointer sent in to tobii_enumerate_face_types.

*user_data* custom pointer which will be passed unmodified to the receiver function.


### Return value

If the operation is successful, tobii_enumerate_face_types returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_enumerate_face_types returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *receiver* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_enumerate_face_types from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The eye tracker does not support enumeration of face types.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support


### See also

tobii_get_face_type() and tobii_set_face_type()


### Example

TBD - example needs to be written.

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_set_face_type( tobii_device_t* device, tobii_face_type_t const face_type )
@ingroup tobii_advanced

tobii_set_face_type
----------------------

### Function

Applies the specified face type setting to the device.


### Syntax

    #include <tobii/tobii_advanced.h>
    tobii_error_t tobii_set_face_type( tobii_device_t* device, tobii_face_type_t const face_type );


### Remarks

Applying a new face type causes the current personal calibration to be discarded and the tracker will revert to the
built-in default calibration for the given face type. A *TOBII_NOTIFICATION_TYPE_FACE_TYPE_CHANGED* will be broadcasted
to all clients notifying them that the face type has changed and that a new calibration has to be set.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*face_type* is a zero-terminated string representation of a specific face type setting, with a maximum length of 63
characters. Supported string values can be queried by calling the tobii_enumerate_face_types() function.


### Return value

If the operation is successful, tobii_set_face_type returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_set_face_type returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *receiver* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_set_face_type from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_OPERATION_FAILED**

    The function failed because it was called while the device was in calibration mode.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The device firmware has no support for setting face type.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support


### See also

tobii_get_face_type() and tobii_enumerate_face_types()


### Example

TBD - example needs to be written.

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_get_face_type( tobii_device_t* device, tobii_face_type_t* face_type );
@ingroup tobii_advanced

tobii_get_face_type
----------------------

### Function

Retreives the current face type setting of the device.


### Syntax

    #include <tobii/tobii_advanced.h>
    tobii_error_t tobii_get_face_type( tobii_device_t* device, tobii_face_type_t* face_type );


### Remarks

A face type is here understood as a class of appearances of the face itself, such as a group of species (e.g. human or
crocodile), facial features (e.g. moustache or makeup), or worn objects (e.g. glasses or hats). It is only used for
situations were auto detecting such differences is difficult or dangerous.

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*face_type* is a pointer to a zero-terminated string representation of the current face type setting,
with a maximum length of 63 characters.


### Return value

If the operation is successful, tobii_get_face_type returns **TOBII_ERROR_NO_ERROR**. If the call fails,
tobii_get_face_type returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *face_type* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_face_type from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_NOT_SUPPORTED**

    The device firmware has no support for retreiving the current face type.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support


### See also

tobii_set_face_type() and tobii_enumerate_face_types()


### Example

TBD - example needs to be written.

*/
