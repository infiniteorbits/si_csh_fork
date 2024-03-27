csp init
apm load

param_server startv2
ping -t 500 153

cmd new set test
cmd add csp_print_packet 1
cmd server upload -v 2
cmd done

cmd new set test2
cmd add csp_print_packet 0
cmd server upload -v 2
cmd done

cmd server list -v 2

# Download command 'test2'
cmd server download -v 2 0x3221D32B

# Remove command 'test'
cmd server rm -v 2 0x888312E

cmd server list -v 2

# Enable print in 5 sec
cmd new set testsch
cmd add csp_print_packet 1
schedule push -v 2 5
cmd done

cmd server list -v 2

# Disable CSP print (command 'test2') in 10 sec
schedule cmd -v 2 0x3221D32B 10

cmd server list -v 2

schedule rm -a

# Enable ping to check CSP print
watch ping
