function decodeCscMeasurement(hexData) {
    let data = hexToBytes(hexData);

    let flags = data[0];
    let crankRevolutionPresent = flags & 0b10;
    let lastCrankEventTimePresent = flags & 0b10;

    let crankRevolution = null;
    let lastCrankEventTime = null;

    if (crankRevolutionPresent) {
        crankRevolution = data[1] + (data[2] << 8);
    }

    if (lastCrankEventTimePresent) {
        lastCrankEventTime = data[3] + (data[4] << 8);
    }

    let crankArray = {
        crankRevolutions: crankRevolution,
        lastCrankEventTime: lastCrankEventTime
    };

    return crankArray;
}

function hexToBytes(hex) {
    let bytes = [];
    for (let i = 0; i < hex.length; i += 2) {
        bytes.push(parseInt(hex.substr(i, 2), 16));
    }
    return bytes;
}

let decoded = decodeCscMeasurement("022001732E");

for (let key in decoded) {
    if (decoded.hasOwnProperty(key)) {
        console.log(key + ': ' + decoded[key]);
    }
}
