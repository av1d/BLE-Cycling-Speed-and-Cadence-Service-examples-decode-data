<?php

function decode_csc_measurement($hex_data) {
    $data = hex2bin($hex_data);

    $flags = ord($data[0]);
    $crank_revolution_present = $flags & 0b10;
    $last_crank_event_time_present = $flags & 0b10;

    $crank_revolution = null;
    $last_crank_event_time = null;

    if ($crank_revolution_present) {
        $crank_revolution = unpack('v', substr($data, 1, 2))[1];
    }

    if ($last_crank_event_time_present) {
        $last_crank_event_time = unpack('v', substr($data, 3, 2))[1];
    }

    $crank_array = array(
        "crank_revolutions" => $crank_revolution,
        "last_crank_event_time" => $last_crank_event_time
    );

    return $crank_array;
}

$decoded = decode_csc_measurement("022001732E");

foreach ($decoded as $key => $value) {
    echo $key . ': ' . $value . '<br>';
}
