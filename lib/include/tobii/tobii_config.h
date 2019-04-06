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

#ifndef tobii_config_h_included
#define tobii_config_h_included

#include "tobii.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum tobii_calibration_point_status_t
{
    TOBII_CALIBRATION_POINT_STATUS_FAILED_OR_INVALID,
    TOBII_CALIBRATION_POINT_STATUS_VALID_BUT_NOT_USED_IN_CALIBRATION,
    TOBII_CALIBRATION_POINT_STATUS_VALID_AND_USED_IN_CALIBRATION,
} tobii_calibration_point_status_t;

typedef struct tobii_calibration_point_data_t
{
    float point_xy[ 2 ];

    tobii_calibration_point_status_t left_status;
    float left_mapping_xy[ 2 ];

    tobii_calibration_point_status_t right_status;
    float right_mapping_xy[ 2 ];
} tobii_calibration_point_data_t;


TOBII_API tobii_error_t TOBII_CALL tobii_calibration_start( tobii_device_t* device, tobii_enabled_eye_t enabled_eye );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_stop( tobii_device_t* device );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_collect_data_2d( tobii_device_t* device,
    float x, float y );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_collect_data_3d( tobii_device_t* device,
    float x, float y, float z );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_collect_data_per_eye_2d( tobii_device_t* device,
    float x, float y, tobii_enabled_eye_t requested_eyes, tobii_enabled_eye_t* collected_eyes );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_discard_data_2d( tobii_device_t* device,
    float x, float y );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_discard_data_3d( tobii_device_t* device,
    float x, float y, float z );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_discard_data_per_eye_2d( tobii_device_t* device,
    float x, float y, tobii_enabled_eye_t eyes );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_clear( tobii_device_t* device );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_compute_and_apply( tobii_device_t* device );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_compute_and_apply_per_eye( tobii_device_t* device, 
    tobii_enabled_eye_t* calibrated_eyes );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_retrieve( tobii_device_t* device,
    tobii_data_receiver_t receiver, void* user_data );
typedef void (*tobii_calibration_point_data_receiver_t)( tobii_calibration_point_data_t const* point_data, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_calibration_parse( tobii_api_t* api, void const* data, size_t data_size,
    tobii_calibration_point_data_receiver_t receiver, void* user_data );

TOBII_API tobii_error_t TOBII_CALL tobii_calibration_apply( tobii_device_t* device,
    void const* data, size_t size );


typedef struct tobii_geometry_mounting_t
{
    int guides;
    float width_mm;
    float angle_deg;
    float external_offset_mm_xyz[ 3 ];
    float internal_offset_mm_xyz[ 3 ];
} tobii_geometry_mounting_t;

TOBII_API tobii_error_t TOBII_CALL tobii_get_geometry_mounting( tobii_device_t* device,
    tobii_geometry_mounting_t* geometry_mounting );


TOBII_API tobii_error_t TOBII_CALL tobii_get_display_area( tobii_device_t* device,
    tobii_display_area_t* display_area );
TOBII_API tobii_error_t TOBII_CALL tobii_set_display_area( tobii_device_t* device,
    tobii_display_area_t const* display_area );
TOBII_API tobii_error_t TOBII_CALL tobii_calculate_display_area_basic( tobii_api_t* api,
    float width_mm, float height_mm, float offset_x_mm, tobii_geometry_mounting_t const* geometry_mounting,
    tobii_display_area_t* display_area );

typedef char tobii_device_name_t[ 64 ];
TOBII_API tobii_error_t TOBII_CALL tobii_get_device_name( tobii_device_t* device,
    tobii_device_name_t* device_name );
TOBII_API tobii_error_t TOBII_CALL tobii_set_device_name( tobii_device_t* device,
    tobii_device_name_t const device_name );

typedef void (*tobii_output_frequency_receiver_t)( float output_frequency, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_enumerate_output_frequencies( tobii_device_t* device,
    tobii_output_frequency_receiver_t receiver, void* user_data );
TOBII_API tobii_error_t TOBII_CALL tobii_set_output_frequency( tobii_device_t* device,
    float output_frequency );
TOBII_API tobii_error_t TOBII_CALL tobii_get_output_frequency( tobii_device_t* device,
    float* output_frequency );


#ifdef __cplusplus
}
#endif

#endif /* tobii_config_h_included */

/**
@defgroup tobii_config tobii_config.h

tobii_config.h
==============

The tobii_config.h header file contains functionality to configure the tracker, such as calibration and display area
setup. Note that using the configuration APIs incorrectly may cause some tracker functionality to work incorrectly.
Please refer to the calibration sample for recommendations on how to implement a correct calibration.

All functions in the configuration API which modify state (i.e. everything except get- and enumerate- functions)
require a config level license, and a device created through tobii_device_create_ex.
*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_start( tobii_device_t* device, tobii_enabled_eye_t enabled_eye );
@ingroup tobii_config

tobii_calibration_start
-----------------------

### Function

Starts a calibration, placing the tracker in a state ready to receive data collection requests.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_start( tobii_device_t* device, 
        tobii_enabled_eye_t enabled_eye );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*enabled_eye* must ALWAYS be **TOBII_ENABLED_EYE_BOTH**

### Return value

If the operation is successful, tobii_calibration_start returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calibration_start returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_start from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_ALREADY_STARTED**

    tobii_calibration_start has already been called, and not yet been stopped by calling tobii_calibration_stop.
    A started calibration must always be stopped before a new calibration is started.

-   **TOBII_ERROR_CALIBRATION_BUSY**

    Another client is already calibrating the device. Only one calibration can be running at a time, across all 
    connected clients.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   **TOBII_ERROR_NOT_SUPPORTED**

    A value other than TOBII_ENABLED_EYE_BOTH was passed for the *enabled_eye* parameter.
    
### See also

tobii_calibration_stop(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_3d(), tobii_calibration_collect_data_per_eye_2d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_3d(), tobii_calibration_discard_data_per_eye_2d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply(), tobii_calibration_compute_and_apply_per_eye(), tobii_calibration_retrieve(),
tobii_calibration_parse(), tobii_calibration_apply()


### Example

See tobii_calibration_collect_data_2d().

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_stop( tobii_device_t* device );
@ingroup tobii_config

tobii_calibration_stop
----------------------

### Function

Signals that the calibration process has been completed, and that no further data collection will be requested.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_stop( tobii_device_t* device );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.


### Return value

If the operation is successful, tobii_calibration_stop returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calibration_stop returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_stop from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support


### See also

tobii_calibration_start(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_3d(), tobii_calibration_collect_data_per_eye_2d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_3d(), tobii_calibration_discard_data_per_eye_2d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply(), tobii_calibration_compute_and_apply_per_eye(), tobii_calibration_retrieve(),
tobii_calibration_parse(), tobii_calibration_apply()


### Example

See tobii_calibration_collect_data_2d().

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_collect_data_2d( tobii_device_t* device, float x, float y );
@ingroup tobii_config


tobii_calibration_collect_data_2d
---------------------------------

### Function

Performs data collection for the specified screen coordinate.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_collect_data_2d( tobii_device_t* device, 
        float x, float y );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*x* the x-coordinate (horizontal) of the point to collect calibration data for, in normalized (0 to 1) coordinates.

*y* the y-coordinate (vertical) of the point to collect calibration data for, in normalized (0 to 1) coordinates.

### Return value

If the operation is successful, tobii_calibration_collect_data_2d returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calibration_collect_data_2d returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_collect_data_2d from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.
    
-   **TOBII_ERROR_OPERATION_FAILED**

    The tracker failed to collect a sufficient amount of data. It is recommended to performing the operation again.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support


### See also

tobii_calibration_start(), tobii_calibration_stop(), 
tobii_calibration_collect_data_3d(), tobii_calibration_collect_data_per_eye_2d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_3d(), tobii_calibration_discard_data_per_eye_2d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply(), tobii_calibration_compute_and_apply_per_eye(), tobii_calibration_retrieve(),
tobii_calibration_parse(), tobii_calibration_apply()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_collect_data_3d( tobii_device_t* device, float x, float y, float z );
@ingroup tobii_config

tobii_calibration_collect_data_3d
---------------------------------

### Function

Performs data collection for the specified 3d coordinate.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_collect_data_3d( tobii_device_t* device, 
        float x, float y, float z );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*x* the x-coordinate (horizontal) of the point to collect calibration data for, in normalized (0 to 1) coordinates.

*y* the y-coordinate (vertical) of the point to collect calibration data for, in normalized (0 to 1) coordinates.

*z* the z-coordinate (depth) of the point to collect calibration data for, in millimeters.


### Return value

If the operation is successful, tobii_calibration_collect_data_3d returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calibration_collect_data_3d returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_collect_data_3d from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.
    
-   **TOBII_ERROR_OPERATION_FAILED**

    The tracker failed to collect a sufficient amount of data. It is recommended to performing the operation again.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

### See also

tobii_calibration_start(), tobii_calibration_stop(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_per_eye_2d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_3d(), tobii_calibration_discard_data_per_eye_2d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply(), tobii_calibration_compute_and_apply_per_eye(), tobii_calibration_retrieve(),
tobii_calibration_parse(), tobii_calibration_apply()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_collect_data_per_eye_2d( tobii_device_t* device, float x, float y, tobii_enabled_eye_t requested_eyes, tobii_enabled_eye_t* collected_eyes );
@ingroup tobii_config

tobii_calibration_collect_data_per_eye_2d
-----------------------------------------

### Function

Performs data collection for the specified screen coordinate, for the left, right or both eyes.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_collect_data_per_eye_2d( tobii_device_t* device, 
        float x, float y, tobii_enabled_eye_t requested_eyes, 
        tobii_enabled_eye_t* collected_eyes );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*x* the x-coordinate (horizontal) of the point to collect calibration data for, in normalized (0 to 1) coordinates.

*y* the y-coordinate (vertical) of the point to collect calibration data for, in normalized (0 to 1) coordinates.

*requested_eyes* specifies wich eye to collect data for: **TOBII_ENABLED_EYE_LEFT**, **TOBII_ENABLED_EYE_RIGHT** or
**TOBII_ENABLED_EYE_BOTH**

*collected_eyes* reports back which eye data was successfully collected for: **TOBII_ENABLED_EYE_LEFT**, 
**TOBII_ENABLED_EYE_RIGHT** or **TOBII_ENABLED_EYE_BOTH**


### Return value

If the operation is successful, tobii_calibration_collect_data_per_eye_2d returns **TOBII_ERROR_NO_ERROR**. If the call
fails, tobii_calibration_collect_data_per_eye_2d returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL, or *requested_eyes* was passed in as an invalid enum value.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_collect_data_per_eye_2d from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.
    
-   **TOBII_ERROR_OPERATION_FAILED**

    The tracker failed to collect a sufficient amount of data. It is recommended to performing the operation again.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   **TOBII_ERROR_NOT_SUPPORTED**

    TBD - Documentation needs to be written for this return value


### See also

tobii_calibration_start(), tobii_calibration_stop(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_3d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_3d(), tobii_calibration_discard_data_per_eye_2d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply(), tobii_calibration_compute_and_apply_per_eye(), tobii_calibration_retrieve(),
tobii_calibration_parse(), tobii_calibration_apply()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_discard_data_2d( tobii_device_t* device, float x, float y );
@ingroup tobii_config

tobii_calibration_discard_data_2d
---------------------------------

### Function

Discards all data collected for the specified screen coordinate.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_discard_data_2d( tobii_device_t* device, 
        float x, float y );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*x* the x-coordinate (horizontal) of the point to discard data for, as specified in a prior call to 
tobii_calibration_collect_data_2d.

*y* the y-coordinate (vertical) of the point to discard data for, as specified in a prior call to 
tobii_calibration_collect_data_2d.


### Return value

If the operation is successful, tobii_calibration_discard_data_2d returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calibration_discard_data_2d returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_discard_data_2d from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.
    
-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support


### See also

tobii_calibration_start(), tobii_calibration_stop(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_3d(), tobii_calibration_collect_data_per_eye_2d(), 
tobii_calibration_discard_data_3d(), tobii_calibration_discard_data_per_eye_2d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply(), tobii_calibration_compute_and_apply_per_eye(), tobii_calibration_retrieve(),
tobii_calibration_parse(), tobii_calibration_apply()


### Example

See tobii_calibration_collect_data_2d().

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_discard_data_3d( tobii_device_t* device, float x, float y, float z );
@ingroup tobii_config

tobii_calibration_discard_data_3d
---------------------------------

### Function

Discards all data collected for the specified 3d coordinate.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_discard_data_3d( tobii_device_t* device, 
        float x, float y, float z );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*x* the x-coordinate (horizontal) of the point to discard data for, as specified in a prior call to 
tobii_calibration_collect_data_3d.

*y* the y-coordinate (vertical) of the point to discard data for, as specified in a prior call to 
tobii_calibration_collect_data_3d.

*z* the z-coordinate (depth) of the point to discard data for, as specified in a prior call to 
tobii_calibration_collect_data_3d.


### Return value

If the operation is successful, tobii_calibration_discard_data_3d returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calibration_discard_data_3d returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_discard_data_3d from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.
    
-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support


### See also

tobii_calibration_start(), tobii_calibration_stop(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_3d(), tobii_calibration_collect_data_per_eye_2d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_per_eye_2d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply(), tobii_calibration_compute_and_apply_per_eye(), tobii_calibration_retrieve(),
tobii_calibration_parse(), tobii_calibration_apply()


### Example

See tobii_calibration_collect_data_3d().

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_discard_data_per_eye_2d( tobii_device_t* device, float x, float y, tobii_enabled_eye_t eyes );
@ingroup tobii_config

tobii_calibration_discard_data_per_eye_2d
-----------------------------------------

### Function

Discards all data collected by a corresponding call to tobii_calibration_collect_data_per_eye_2d.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_discard_data_per_eye_2d( tobii_device_t* device, 
        float x, float y, tobii_enabled_eye_t eyes );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*x* the x-coordinate (horizontal) of the point to discard data for, as specified in a prior call to 
tobii_calibration_collect_data_per_eye_2d.

*y* the y-coordinate (vertical) of the point to discard data for, as specified in a prior call to 
tobii_calibration_collect_data_per_eye_2d.

*eyes* specifies wich eye to discard data for: **TOBII_ENABLED_EYE_LEFT**, **TOBII_ENABLED_EYE_RIGHT** or
**TOBII_ENABLED_EYE_BOTH**, which should match the value passed in the corresonding 
tobii_calibration_collect_data_per_eye_2d call.


### Return value

If the operation is successful, tobii_calibration_discard_data_per_eye_2d returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calibration_discard_data_per_eye_2d returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL, *eyes* was passed in as an invalid enum value.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_discard_data_per_eye_2d from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.
    
-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   **TOBII_ERROR_NOT_SUPPORTED**

    TBD - Documentation needs to be written for this return value


### See also

tobii_calibration_start(), tobii_calibration_stop(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_3d(), tobii_calibration_collect_data_per_eye_2d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_3d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply(), tobii_calibration_compute_and_apply_per_eye(), tobii_calibration_retrieve(),
tobii_calibration_parse(), tobii_calibration_apply()


### Example

See tobii_calibration_collect_data_per_eye_2d().

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_clear( tobii_device_t* device );
@ingroup tobii_config

tobii_calibration_clear
-----------------------

### Function

Resets the calibration. Also performed internally when calling tobii_calibration_start.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_clear( tobii_device_t* device );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.


### Return value

If the operation is successful, tobii_calibration_clear returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calibration_clear returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_clear from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.
    
-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support


### See also

tobii_calibration_start(), tobii_calibration_stop(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_3d(), tobii_calibration_collect_data_per_eye_2d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_3d(), tobii_calibration_discard_data_per_eye_2d(), 
tobii_calibration_compute_and_apply(), tobii_calibration_compute_and_apply_per_eye(), tobii_calibration_retrieve(),
tobii_calibration_parse(), tobii_calibration_apply()



### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_compute_and_apply( tobii_device_t* device );
@ingroup tobii_config

tobii_calibration_compute_and_apply
-----------------------------------

### Function

Computes a calibration based on data collected so far, and applies the resulting calibration to the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_compute_and_apply( tobii_device_t* device );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.


### Return value

If the operation is successful, tobii_calibration_compute_and_apply returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calibration_compute_and_apply returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_compute_and_apply from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_OPERATION_FAILED**

    Not enough data collected to compute calibration.
    
-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support



### See also

tobii_calibration_start(), tobii_calibration_stop(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_3d(), tobii_calibration_collect_data_per_eye_2d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_3d(), tobii_calibration_discard_data_per_eye_2d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply_per_eye(), tobii_calibration_retrieve(),
tobii_calibration_parse(), tobii_calibration_apply()



### Example

See tobii_calibration_collect_data_2d().

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_compute_and_apply_per_eye( tobii_device_t* device, tobii_enabled_eye_t* calibrated_eyes );
@ingroup tobii_config

tobii_calibration_compute_and_apply_per_eye
-------------------------------------------

### Function

Computes a calibration based on data collected so far, using tobii_calibration_collect_data_per_eye_2d, and applies 
the resulting calibration to the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_compute_and_apply_per_eye( tobii_device_t* device, 
        tobii_enabled_eye_t* calibrated_eyes );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*calibrated_eyes* receives information about which eyes were successfully calibrated: **TOBII_ENABLED_EYE_LEFT**, 
**TOBII_ENABLED_EYE_RIGHT** or **TOBII_ENABLED_EYE_BOTH**


### Return value

If the operation is successful, tobii_calibration_compute_and_apply_per_eye returns **TOBII_ERROR_NO_ERROR**. If the 
call fails, tobii_calibration_compute_and_apply_per_eye returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_compute_and_apply_per_eye from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_NOT_STARTED**

    A successful call to tobii_calibration_start has not been made before calling this function.

-   **TOBII_ERROR_OPERATION_FAILED**

    Not enough data collected to compute calibration.
    
-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support


### See also

tobii_calibration_start(), tobii_calibration_stop(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_3d(), tobii_calibration_collect_data_per_eye_2d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_3d(), tobii_calibration_discard_data_per_eye_2d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply(), tobii_calibration_retrieve(),
tobii_calibration_parse(), tobii_calibration_apply()


### Example

See tobii_calibration_collect_data_per_eye_2d().

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_retrieve( tobii_device_t* device, tobii_data_receiver_t receiver, void* user_data );
@ingroup tobii_config

tobii_calibration_retrieve
--------------------------

### Function

Retrieves the currently applied calibration from the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_retrieve( tobii_device_t* device, 
        tobii_data_receiver_t receiver, void* user_data );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*receiver* is a function pointer to a function with the prototype:

    void data_receiver( void const* data, size_t size, void* user_data )

This function will be called with the retrieved calibration data. It is called with the following parameters:

-   *data*
    The calibration data read from device. This pointer will be invalid after returning from the function,
    so ensure you make a copy of the data rather than storing the pointer directly.

-   *size*
    The size of the calibration data, in bytes.

-   *user_data*
    This is the custom pointer passed to tobii_calibration_retrieve.

*user_data* custom pointer which will be passed unmodified to the receiver function.


### Return value

If the operation is successful, tobii_calibration_retrieve returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calibration_retrieve returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *receiver* parameter was passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_retrieve from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   TBD - Other possible error values currently unknown


### See also

tobii_calibration_start(), tobii_calibration_stop(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_3d(), tobii_calibration_collect_data_per_eye_2d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_3d(), tobii_calibration_discard_data_per_eye_2d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply(), tobii_calibration_compute_and_apply_per_eye(), 
tobii_calibration_parse(), tobii_calibration_apply()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_parse( tobii_api_t* api, void const* data, size_t data_size, tobii_calibration_point_data_receiver_t receiver, void* user_data );
@ingroup tobii_config

tobii_calibration_parse
-----------------------

### Function

Extracts data about calibration points from the specified calibration.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_parse( tobii_api_t* api, void const* data, 
        size_t data_size, tobii_calibration_point_data_receiver_t receiver, 
        void* user_data );


### Remarks

TBD - Documentation needs to be written for this function

*api* must be a pointer to a valid tobii_api_t instance as created by calling tobii_api_create.

*data* is the calibration data retrieved by tobii_calibration_retrieve().

*data_size* is the size of the data retrieved by tobii_calibration_retrieve().

*receiver* is a function pointer to a function with the prototype:

    void receiver( tobii_calibration_point_data_t const* point_data, void* user_data )

This function will be called for each parsed point from the calibration. It is called with the following parameters:

-   *point_data*
    A pointer to a struct containing all the data related to a calibration point. 
    TBD - document the meaning of each field

-   *user_data*
    This is the custom pointer sent in to tobii_calibration_parse.

*user_data* custom pointer which will be passed unmodified to the receiver function.


### Return value

If the operation is successful, tobii_calibration_parse returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calibration_parse returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *api*, *data* or *receiver* parameters were passed in as NULL, or *data_size* parameter was less than 8.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_parse from within a callback function is not supported.

-   **TOBII_OPERATION_FAILED**
    
    The data being parsed was not a valid calibration.

### See also

tobii_calibration_start(), tobii_calibration_stop(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_3d(), tobii_calibration_collect_data_per_eye_2d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_3d(), tobii_calibration_discard_data_per_eye_2d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply(), tobii_calibration_compute_and_apply_per_eye(), tobii_calibration_retrieve(),
tobii_calibration_apply()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calibration_apply( tobii_device_t* device, void const* data, size_t size );
@ingroup tobii_config

tobii_calibration_apply
-----------------------

### Function

Applies the specified calibration to the device, making it the current calibration.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calibration_apply( tobii_device_t* device, 
        void const* data, size_t size );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*data* is the calibration data which has previously been retrieved by calling tobii_calibration_retrieve()

*size* is the size of the calibration data which has previously been retrieved by calling tobii_calibration_retrieve()


### Return value

If the operation is successful, tobii_calibration_apply returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calibration_apply returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *data* parameters were passed in as NULL, or the *data_size* parameter was not greater than 0.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_calibration_apply from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_CONNECTION_FAILED**

    The connection to the device was lost. Call tobii_device_reconnect() to re-establish connection.

-   **TOBII_ERROR_CALIBRATION_BUSY**

    The device is currently being calibrated. tobii_calibration_apply can not be called while calibrating the device.

-   **TOBII_ERROR_OPERATION_FAILED**

    The provided calibration could not be applied to the device.
    
-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support


### See also

tobii_calibration_start(), tobii_calibration_stop(), tobii_calibration_collect_data_2d(), 
tobii_calibration_collect_data_3d(), tobii_calibration_collect_data_per_eye_2d(), tobii_calibration_discard_data_2d(),
tobii_calibration_discard_data_3d(), tobii_calibration_discard_data_per_eye_2d(), tobii_calibration_clear(),
tobii_calibration_compute_and_apply(), tobii_calibration_compute_and_apply_per_eye(), tobii_calibration_retrieve(),
tobii_calibration_parse()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_get_geometry_mounting( tobii_device_t* device, tobii_geometry_mounting_t* geometry_mounting );
@ingroup tobii_config

tobii_get_geometry_mounting
---------------------------

### Function

Retrieves the geometry mounting of the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_get_geometry_mounting( tobii_device_t* device, 
        tobii_geometry_mounting_t* geometry_mounting );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*geometry_mounting* must be a valid pointer to a tobii_geometry_mounting_t instance which will receive the result.


### Return value

If the operation is successful, tobii_get_geometry_mounting returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_get_geometry_mounting returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *geometry_mounting* parameters were passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_geometry_mounting from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blacklisted.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   TBD - Other possible error values currently unknown


### See also

tobii_calculate_display_area_basic()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_get_display_area( tobii_device_t* device, tobii_display_area_t* display_area );
@ingroup tobii_config

tobii_get_display_area
----------------------

### Function

Retrieves the current display area from the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_get_display_area( tobii_device_t* device, 
        tobii_display_area_t* display_area );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*display_area* must be a valid pointer to a tobii_display_area_t instance which will receive the result.


### Return value

If the operation is successful, tobii_get_display_area returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_get_display_area returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *display_area* parameters were passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_display_area from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blacklisted.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   TBD - Other possible error values currently unknown


### See also

tobii_set_display_area(), tobii_calculate_display_area_basic()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_set_display_area( tobii_device_t* device, tobii_display_area_t const* display_area );
@ingroup tobii_config

tobii_set_display_area
----------------------

### Function

Applies the specified display area setting to the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_set_display_area( tobii_device_t* device, 
        tobii_display_area_t const* display_area );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*display_area* must be a valid pointer to a tobii_display_area_t which is correctly initialize, for example by callin
tobii_calculate_display_area_basic().


### Return value

If the operation is successful, tobii_set_display_area returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_set_display_area returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *display_area* parameters were passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_set_display_area from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   TBD - Other possible error values currently unknown


### See also

tobii_get_display_area(), tobii_calculate_display_area_basic()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_calculate_display_area_basic( tobii_api_t* api, float width_mm, float height_mm, float offset_x_mm, tobii_geometry_mounting_t const* geometry_mounting, tobii_display_area_t* display_area );
@ingroup tobii_config

tobii_calculate_display_area_basic
----------------------------------

### Function

Calculates a basic display area configuration based on screen size and geometry mounting.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_calculate_display_area_basic( tobii_api_t* api, 
        float width_mm, float height_mm, float offset_x_mm, 
        tobii_geometry_mounting_t const* geometry_mounting, 
        tobii_display_area_t* display_area );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*width_mm* is the width of the display device in millimeters.

*height_mm* is the height of the display device in millimeters.

*offset_x* is the offset of the eye tracker from the center of the display device in the x-axis, in millimeters.

*geometry_mounting* is the geometry mounting information as received by tobii_get_geometry_mounting()

*display_area* must be a valid pointer to a tobii_display_area_t instance which will receive the result.


### Return value

If the operation is successful, tobii_calculate_display_area_basic returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_calculate_display_area_basic returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *api*, *geometry_mounting* or *display_area* parameters was passed in as NULL.


### See also

tobii_get_display_area(), tobii_get_geometry_mounting(), 


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_get_device_name( tobii_device_t* device, tobii_device_name_t* device_name );
@ingroup tobii_config

tobii_get_device_name
---------------------

### Function

Retrieves the users nickname for the device, if it has been set.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_get_device_name( tobii_device_t* device, 
        tobii_device_name_t* device_name );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*device_name*  must be a valid pointer to a tobii_device_name_t instance which will receive the result.


### Return value

If the operation is successful, tobii_get_device_name returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_get_device_name returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *device_name* parameters were passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_device_name from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blacklisted.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   TBD - Other possible error values currently unknown


### See also

tobii_set_device_name()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_set_device_name( tobii_device_t* device, tobii_device_name_t const device_name );
@ingroup tobii_config

tobii_set_device_name
---------------------

### Function

Sets a user nickname for the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_set_device_name( tobii_device_t* device, 
        tobii_device_name_t const device_name );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*device_name*  must be a valid pointer to a tobii_device_name_t instance containing the name to be applied.


### Return value

If the operation is successful, tobii_set_device_name returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_set_device_name returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *device_name* parameters were passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_set_device_name from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   TBD - Other possible error values currently unknown


### See also

tobii_get_device_name()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_enumerate_output_frequencies( tobii_device_t* device, tobii_output_frequency_receiver_t receiver, void* user_data );
@ingroup tobii_config

tobii_enumerate_output_frequencies
----------------------------------

### Function

Lists all valid output frequencies for the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_enumerate_output_frequencies( tobii_device_t* device, 
        tobii_output_frequency_receiver_t receiver, void* user_data );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*receiver* is a function pointer to a function with the prototype:

    void receiver( ( float output_frequency, void* user_data ) )

This function will be called for each available output frequency. It is called with the following parameters:

-   *output_frequency*
    The frequency in Hz.

-   *user_data*
    This is the custom pointer sent in to tobii_enumerate_output_frequencies.

*user_data* custom pointer which will be passed unmodified to the receiver function.


### Return value

If the operation is successful, tobii_enumerate_output_frequencies returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_enumerate_output_frequencies returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *receiver* parameters were passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_geometry_mounting from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blacklisted.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   TBD - Other possible error values currently unknown


### See also

tobii_set_output_frequency(), tobii_get_output_frequency()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_set_output_frequency( tobii_device_t* device, float output_frequency );
@ingroup tobii_config

tobii_set_output_frequency
--------------------------

### Function

Configures the device to run at the specified output frequency.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_set_output_frequency( tobii_device_t* device, 
        float output_frequency );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*output_frequency* the frequency to apply.


### Return value

If the operation is successful, tobii_set_output_frequency returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_set_output_frequency returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* parameters were passed in as NULL, or *output_frequency* is lower than 0.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_set_output_frequency from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid config level license, or has been blacklisted.

-   **TOBII_ERROR_OPERATION_FAILED**

    The function failed because it was called while the device was in calibration mode.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   TBD - Other possible error values currently unknown


### See also

tobii_get_output_frequency(), tobii_enumerate_output_frequencies()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/

/**
@fn TOBII_API tobii_error_t TOBII_CALL tobii_get_output_frequency( tobii_device_t* device, float* output_frequency );
@ingroup tobii_config

tobii_get_output_frequency
--------------------------

### Function

Queries the current output frequency of the device.


### Syntax

    #include <tobii/tobii_config.h>
    tobii_error_t tobii_get_output_frequency( tobii_device_t* device, 
        float* output_frequency );


### Remarks

TBD - Documentation needs to be written for this function

*device* must be a pointer to a valid tobii_device_t instance as created by calling tobii_device_create.

*output_frequency* is a valid pointer to a float which will receive the current output frequency.

### Return value

If the operation is successful, tobii_get_output_frequency returns **TOBII_ERROR_NO_ERROR**. If the call fails, 
tobii_get_output_frequency returns one of the following:

-   **TOBII_ERROR_INVALID_PARAMETER**

    The *device* or *output_frequency* parameters were passed in as NULL.

-   **TOBII_ERROR_CALLBACK_IN_PROGRESS**

    The function failed because it was called from within a callback triggered from an API call such as 
    tobii_device_process_callbacks(), tobii_calibration_retrieve() or tobii_enumerate_illumination_modes().
    Calling tobii_get_output_frequency from within a callback function is not supported.

-   **TOBII_ERROR_INSUFFICIENT_LICENSE**

    The provided license was not a valid license, or has been blacklisted.

-   **TOBII_ERROR_INTERNAL**

    Some unexpected internal error occurred. This error should normally not be returned, so if it is, please contact
    the support

-   TBD - Other possible error values currently unknown


### See also

tobii_set_output_frequency(), tobii_enumerate_output_frequencies()


### Example

@code{.c}

    #include <tobii/tobii_config.h>

    int main()
    {
        // TODO: Implement example
    }

@endcode

*/
