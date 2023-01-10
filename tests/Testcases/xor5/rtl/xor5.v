module xor5(
  a,
  out);

input wire [4:0] a;
output wire out;
  `pragma protect begin_protected
`pragma protect author = "Verific"
`pragma protect author_info = "Verific Corporation"
`pragma protect data_method = "aes128-cbc"
`pragma protect key_keyowner = "Verific"
`pragma protect key_keyname = "key1"
`pragma protect key_method = "rsa"
`pragma protect encoding = (enctype = "base64", line_length = 64, bytes = 128), key_block
F4u+vmSjeR5iyb8Gg0v/1UkRL44XXrhfLU1zPBxujxQ5YkNmhffzqSn6cqoDSzzM
6oq49VUbE7xlvv0ZP5wBwpDGNaI27wzaU2AJv6cHc7YpaJpRWTCFgjYOvSEfgJDi
Z3oiUYm2Hdj9czIQcDa3BbQcUAqZGpmJUMWsnDVdkc0=
`pragma protect encoding = (enctype = "base64", line_length = 64, bytes = 128), data_block
adZ3bhMjiYoszIl8qAFwDKok8boWXomc5En3xbKsLET7vJh1+XVa8vX6nmiweMnm
0pnYRVSIG98KM7TsxMJcEQ==
`pragma protect end_protected

endmodule
