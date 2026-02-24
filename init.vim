" init.vim - minimal, complete config

" Load vim-plug plugin manager
call plug#begin('~/.local/share/nvim/plugged')

Plug 'neovim/nvim-lspconfig'
Plug 'nvim-treesitter/nvim-treesitter', {'do': ':TSUpdate'}
Plug 'L3MON4D3/LuaSnip'
Plug 'nvim-tree/nvim-tree.lua'
Plug 'nvim-tree/nvim-web-devicons'
Plug 'lewis6991/gitsigns.nvim'
Plug 'nvim-lualine/lualine.nvim' " Status line
Plug 'catppuccin/nvim', {'as': 'catppuccin'}
Plug 'morhetz/gruvbox', {'as': 'gruvbox'}
Plug 'nvim-lua/plenary.nvim'
Plug 'nvim-telescope/telescope.nvim'
Plug 'neovim/nvim-lspconfig' " LSP config
Plug 'hrsh7th/nvim-cmp'
Plug 'hrsh7th/cmp-nvim-lsp'
Plug 'hrsh7th/cmp-buffer'
Plug 'hrsh7th/cmp-path'
Plug 'hrsh7th/cmp-cmdline'
Plug 'saadparwaiz1/cmp_luasnip'
Plug 'rafamadriz/friendly-snippets'
Plug 'utilyre/barbecue.nvim'
Plug 'SmiteshP/nvim-navic'
Plug 'tpope/vim-fugitive'
Plug 'yuttie/comfortable-motion.vim'

call plug#end()

" Basic options
set number
set relativenumber
set tabstop=4
set shiftwidth=4
set expandtab
set termguicolors
syntax enable
set background=dark
colorscheme catppuccin

" Treesitter config
lua << EOF
require'nvim-treesitter.configs'.setup {
  ensure_installed = { "c", "cpp", "bash", "lua" , "json" , "vim" },
  highlight = { enable = true },
}
EOF

" LSP config
lua << EOF
local lspconfig = require'lspconfig'
local cmp_nvim_lsp = require'cmp_nvim_lsp'

local capabilities = cmp_nvim_lsp.default_capabilities()

lspconfig.clangd.setup { capabilities = capabilities }
lspconfig.bashls.setup { capabilities = capabilities }
EOF

" nvim-cmp setup
lua << EOF
local cmp = require'cmp'
local luasnip = require'luasnip'

cmp.setup {
  snippet = {
    expand = function(args)
      luasnip.lsp_expand(args.body)
    end,
  },
  mapping = {
    ['<C-Space>'] = cmp.mapping.complete(),
    ['<CR>'] = cmp.mapping.confirm { select = false },
    ['<Tab>'] = cmp.mapping.select_next_item(),
    ['<S-Tab>'] = cmp.mapping.select_prev_item(),
  },
  sources = {
    { name = 'nvim_lsp' },
    { name = 'buffer' },
    { name = 'path' }
  },
}
EOF

" Nvim-tree setup and toggle keymap
lua << EOF
require'nvim-tree'.setup {}
EOF

" Gitsigns setup
lua << EOF
require'gitsigns'.setup {}
EOF

" LSP keymaps
" Set leader key (optional, if not already set)
let mapleader = " "
nnoremap <leader>e :NvimTreeToggle<CR>
nnoremap gd <cmd>lua vim.lsp.buf.definition()<CR>
nnoremap gv <cmd>lua vim.cmd("vsplit"); vim.lsp.buf.definition()<CR>
nnoremap <leader>b  <cmd>lua vim.lsp.buf.hover()<CR>
nnoremap <leader>rn <cmd>lua vim.lsp.buf.rename()<CR>
nnoremap <leader>a <cmd>lua vim.lsp.buf.code_action()<CR>
nnoremap K :lua vim.diagnostic.open_float()<CR>
nnoremap ]d :lua vim.diagnostic.goto_next()<CR>
nnoremap [d :lua vim.diagnostic.goto_prev()<CR>

" Make line numbers brighter
highlight LineNr ctermfg=LightGrey guifg=#aaaaaa

lua << EOF
require('lspconfig').clangd.setup {
  cmd = { "clangd", "--compile-commands-dir=." }
}
EOF
lua << EOF
local telescope = require('telescope')
local builtin = require('telescope.builtin')

telescope.setup{
  defaults = {
    prompt_prefix = "🔍 ",
    selection_caret = "➤ ",
    sorting_strategy = "ascending",
    layout_config = {
      prompt_position = "top",
    },
  }
}

-- Define a global function so Vimscript can call it
function _G.grep_yanked()
  local text = vim.fn.getreg('"') -- last yanked text
  builtin.live_grep({ default_text = text })
end
EOF

" ==============================
" Telescope Keymaps
" ==============================
nnoremap <leader>ff <cmd>Telescope find_files<cr>
nnoremap <silent> <leader>gy :lua grep_yanked()<CR>
nnoremap <leader>fb <cmd>Telescope buffers<cr>
nnoremap <leader>fh <cmd>Telescope help_tags<cr>

" Optional: Visual mode mapping (yank + search)
xnoremap <silent> <leader>gy y:lua grep_yanked()<CR>

" ALE Linting 
let g:ale_linters = {
            \ 'sh': ['shellcheck'],
            \}
let g:ale_fixers = {
            \ 'sh': ['shfmt'],
            \}
let g:ale_fix_on_save = 1


lua << EOF
local navic = require("nvim-navic")
local lspconfig = require("lspconfig")

lspconfig.clangd.setup {
    on_attach = function(client, bufnr)
     navic.attach(client, bufnr)
    end,
}
lspconfig.pyright.setup {
  on_attach = function(client, bufnr)
    navic.attach(client, bufnr)
  end,
}
EOF

lua << EOF
require("barbecue").setup {
    create_autocmd = false,
}
EOF
" Terminal mode mappings
nnoremap <S-Left>  <C-w>h
nnoremap <S-Down>  <C-w>j
nnoremap <S-Up>    <C-w>k
nnoremap <S-Right> <C-w>l
nnoremap <S-q>      :q!<CR>
nnoremap <S-x>      :x!<CR>
nnoremap <leader>ff :Telescope find_files hidden=true<CR>
nnoremap <leader>lg :Telescope live_grep<CR>
nnoremap <C-s> :w<CR>
set winminwidth=20
set winminheight=1
set equalalways

set winbar=%{%v:lua.require'nvim-navic'.get_location()%}

"set laststatus=2
"set statusline=%{%v:lua.require'nvim-navic'.get_location()%}
lua << EOF
local actions = require("telescope.actions")

require("telescope").setup{
  defaults = {
    mappings = {
      i = {   -- insert mode
        ["<Tab>"] = actions.select_vertical,
      },
      n = {   -- normal mode
        ["<Tab>"] = actions.select_vertical,
      },
    },
  }
}
EOF
command! W w
command! Wqa wqa
"hi Normal cterm=bold gui=bold guifg=#E0E0E0

" =============================
" Softer syntax highlighting
" =============================
hi Keyword    gui=bold guifg=#808080
hi Function    gui=bold guifg=#808080
hi Identifier guifg=#808080
hi String     cterm=bold gui=bold guifg=#F1FA8C
hi Comment     guifg=#A0A0A0
hi Type        gui=bold guifg=#808080
hi Constant    guifg=#C0C0C0
hi Statement  cterm=bold gui=bold guifg=#FFB86C

" =============================
" Line numbers and cursor line
" =============================
"hi LineNr cterm=bold gui=bold guifg=#B0B0B0
"hi CursorLineNr cterm=bold gui=bold guifg=#50FA7B
highlight Normal ctermbg=black guibg=black guifg=0
highlight NormalNC ctermbg=black guibg=black guifg=0
"set cursorline
" Activate comfortable motion on startup

" Optional key mappings for smooth scroll
nnoremap <C-Up> <Plug>(ComfortableMotionScrollDown)
nnoremap <C-Down> <Plug>(ComfortableMotionScrollUp)
lua << EOF
require("lspconfig").clangd.setup{
  cmd = {
    "clangd",
    "--background-index=false",
    "--clang-tidy=false",
    "-j=2"
  },
}
EOF
"hi CursorLine cterm=bold gui=bold guibg=#2A2A3A
"--background-index=false",
" ================================
" Basic editor behavior
" ================================
set nocompatible
syntax on
set number
set relativenumber
set cursorline
set showcmd
set wildmenu
set lazyredraw
set ttyfast

" ================================
" Indentation
" ================================
set tabstop=4
set shiftwidth=4
set expandtab
set smartindent

" ================================
" Searching
" ================================
set ignorecase
set smartcase
set incsearch
set hlsearch

" ================================
" Colors & UI (Gentle on eyes)
" ================================
set termguicolors
set background=dark

" Clear existing highlights
hi clear
if exists("syntax_on")
  syntax reset
endif

" Custom soft night colorscheme
let g:colors_name = "gentle_night"

" Editor background & text
hi Normal       guifg=#C8CCD4 guibg=#1B1F27
hi LineNr       guifg=#4C566A guibg=#1B1F27
hi CursorLineNr guifg=#8FA1B3 guibg=#1B1F27
hi CursorLine   guibg=#222733

" Status
" Move selected lines down
vnoremap J :m '>+1<CR>gv=gv

" Move selected lines up
vnoremap K :m '<-2<CR>gv=gv
xnoremap // y/\V<C-R>"<CR>

