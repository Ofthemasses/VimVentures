vim9script

set noswapfile
# For now this is fine, though autocomplete is something worth restricting in
# future
def RestrictParentDir()
    var curr_file = expand('%:p')

    var working_dir = expand('~/.vimventures')

    if stridx(curr_file, working_dir) != 0
        echohl WarningMsg
        echo "Warn: you are outside of the ships home drive"
        echohl None

        execute 'bd'
    endif
enddef

var mode_loop = 1

def StartInteractiveMode()
    execute 'enew'
    execute 'file INTERACTIVE'
enddef

def OnData(channel: channel, msg: string): void
    var split_msg = split(msg, "\n")
    var count = 0
    for i in split_msg
        count += 1
        setbufline("INTERACTIVE", count, i)
    endfor
    ch_sendraw(channel, "RECV\0")
enddef

def OnClose(channel: channel): void
	echo 'Channel closed'
enddef

def StartTCPClient(): void
	var server = '127.0.0.1:8080'
	var channel = ch_open(server, {
				\ 'mode': 'raw',
				\ 'callback': function('OnData'),
				\ 'close_cb': function('OnClose'),
                \ 'timeout': 50
				\ })
	if ch_status(channel) == 'open'
		echo 'Connected to the server at ' .. server
	else
		echo 'Failed to connect to server'
	endif
enddef

au BufRead,BufNewFile * call RestrictParentDir()

silent StartInteractiveMode()
silent StartTCPClient()
