def decode_csc_measurement(hex_data):
    data = bytearray.fromhex(hex_data)
    
    flags = data[0]
    crank_revolution_present = flags & 0b10
    last_crank_event_time_present = flags & 0b10

    crank_revolution = None
    last_crank_event_time = None
    
    if crank_revolution_present:
        crank_revolution = int.from_bytes(data[1:3], byteorder='little')
        
    if last_crank_event_time_present:
        last_crank_event_time = int.from_bytes(data[3:5], byteorder='little')
    
    return crank_revolution, last_crank_event_time


"""
Here you will put the data from your sensor. In this case,
I am using 5 octets which contain 'Crank Revolutions' and
'Last Crank Event Time'.
"""
hex_data_example = "022001732E"  # your data

crank_revolution, last_crank_event_time = decode_csc_measurement(hex_data_example)
print(f"Crank Revolutions: {crank_revolution}")
print(f"Last Crank Event Time: {last_crank_event_time}")
