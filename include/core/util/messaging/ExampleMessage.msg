---
  # If this is ommited, generated code will be placed in global namespace.
  namespace: Messages
  # Adds includes on top of generated header file for message if specified.
  # If ommited, no includes will be added, apart from the Messaging.h in this directory.
  includes:
    - "core/util/Util.h"
  notifications:
    -
      name: SensorStateChange
      # If this is ommited, queue is not used
      # and notification is reported as soon as it arrives.
      # Otherwise, flush() needs to be called manually on
      # subscriber.
      queue-size: 16
      # If data is ommited, data_t structure will not be generated.
      data:
      -
        type: "size_t"
        name: "index"
      -
        type: "bool"
        name: "state"
  services:
    -
      name: SensorState
      # If request is ommited, request_t structure will not be generated.
      request:
      -
        type: "size_t"
        name: "index"
      # If response is ommited, response_t structure will not be generated.
      response:
      -
        type: "bool"
        name: "state"
    -
      name: SpecificSensorState
      response:
      -
        type: "bool"
        name: "state"