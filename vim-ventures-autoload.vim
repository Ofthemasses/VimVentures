
" For now this is fine, though autocomplete is something worth restricting in
" future
function RestrictParentDir()
    let l:curr_file = expand('%:p')

    let l:working_dir = expand('~/.vimventures')

    if stridx(l:curr_file, l:working_dir) != 0
        echohl WarningMsg
        echo "Warn: you are outside of the ships home drive"
        echohl None

        execute 'bd'
    endif
endfunction

autocmd BufRead,BufNewFile * call RestrictParentDir()
