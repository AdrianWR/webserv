set number " numeros de linha
" set rnu "relative numbers
" Cor original
"color slate
syntax on
set hlsearch
nnoremap <silent> [b :bprevious<CR>
nnoremap <silent> ]b :bnext<CR>
nnoremap <silent> [B :bfirst<CR>
nnoremap <silent> ]B :blast<CR>
set noerrorbells visualbell t_vb=
set wildmenu
"set expandtab       " replace tabs with spaces
set autoindent      " copy indent when starting a new line
set tabstop=4
set softtabstop=4
set shiftwidth=4
""set colorcolumn=81
map <S-Left> :tabp<cr>
map <S-Right> :tabn<cr>
map <S-Tab> :tabnew<cr>
map <C-a> gg<S-v><S-g>
map :bb :buffers<cr>
set ruler
set cursorline
set suffixesadd+=.c
map	gf <C-w>gf
map p/ :norm i//<cr>
map r/ :norm 2x<cr>
nnoremap gp `[v`]
" Faz BS funcionar no vim 9
" set backspace=indent,eol,start
" " Faz indentlines
set listchars=tab:\|\ 
set list

" Instalacao do Vim-Plug:
" curl -fLo ~/.vim/autoload/plug.vim --create-dirs \
"     https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
" from:
" https://github.com/junegunn/vim-plug
"
call plug#begin()
" The default plugin directory will be as follows:
"   - Vim (Linux/macOS): '~/.vim/plugged'
"   - Vim (Windows): '~/vimfiles/plugged'
"   - Neovim (Linux/macOS/Windows): stdpath('data') . '/plugged'
" You can specify a custom plugin directory by passing it as the argument
"   - e.g. `call plug#begin('~/.vim/plugged')`
"   - Avoid using standard Vim directory names like 'plugin'

" Make sure you use single quotes

Plug 'matsuuu/pinkmare'
"Plug 'ycm-core/YouCompleteMe'
Plug 'scrooloose/nerdtree'
Plug 'vim-airline/vim-airline'
Plug 'airblade/vim-gitgutter'
Plug 'octol/vim-cpp-enhanced-highlight'
"Plug 'rip-rip/clang_complete'
"Plug 'justmao945/vim-clang'

call plug#end()
" You can revert the settings after the call like so:
"   filetype indent off   " Disable file-type-specific indentation
"   syntax off            " Disable syntax highlighting
"
" COr do plugin to terminal root
set termguicolors
colorscheme pinkmare

"NERDTree Configs
nnoremap <leader>n :NERDTreeFocus<CR>
nnoremap <c-n> :NERDTree<CR>
nnoremap '' :NERDTreeToggle<CR>
nnoremap <C-f> :NERDTreeFind<CR>
" Exit Vim if NERDTree is the only window remaining in the only tab.
autocmd BufEnter * if tabpagenr('$') == 1 && winnr('$') == 1 && exists('b:NERDTree') && b:NERDTree.isTabTree() | quit | endif

" Close the tab if NERDTree is the only window remaining in it.
autocmd BufEnter * if winnr('$') == 1 && exists('b:NERDTree') && b:NERDTree.isTabTree() | quit | endif
"Clang Complet Config
"let g:clang_library_path='/usr/lib/libclang.so'
