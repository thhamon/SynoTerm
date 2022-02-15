####################################################################
#
#    This file was generated using Parse::Yapp version 1.05.
#
#        Don't edit this file, use source file instead.
#
#             ANY CHANGE MADE HERE WILL BE LOST !
#
####################################################################
package GNMParser;
use vars qw ( @ISA );
use strict;

@ISA= qw ( Parse::Yapp::Driver );
use Parse::Yapp::Driver;

#line 13 "GNMParser.yp"

    my $debug = 0;

    my $if_word;
    my $lm_word;
    my $dir = 0;
    my @if_stack;
    my @lm_stack;
    my $if_gnm_ref;
    my $first_word;
    my $last_word;
    my $word_in_r = 0;
    my $word_in_l = 0;
    my $word_in = 0;


sub new {
        my($class)=shift;
        ref($class)
    and $class=ref($class);

    my($self)=$class->SUPER::new( yyversion => '1.05',
                                  yystates =>
[
	{#State 0
		DEFAULT => -1,
		GOTOS => {
			'line' => 1
		}
	},
	{#State 1
		ACTIONS => {
			'' => 3,
			'TAG' => -7,
			'error' => 6,
			'OPEN_CT' => 8
		},
		GOTOS => {
			'OPEN' => 2,
			'word' => 5,
			'@1-0' => 4,
			'gnm' => 7
		}
	},
	{#State 2
		ACTIONS => {
			'TAG' => -7,
			'error' => 6,
			'OPEN_CT' => 8
		},
		GOTOS => {
			'OPEN' => 2,
			'word' => 9,
			'@1-0' => 4,
			'gnm' => 10
		}
	},
	{#State 3
		DEFAULT => 0
	},
	{#State 4
		ACTIONS => {
			'TAG' => 12,
			'error' => 13
		},
		GOTOS => {
			'POSTAG' => 11
		}
	},
	{#State 5
		DEFAULT => -5
	},
	{#State 6
		ACTIONS => {
			"\nOPEN: " => 14,
			"\ngnm: " => 15,
			"\nword: " => 16
		}
	},
	{#State 7
		DEFAULT => -2
	},
	{#State 8
		DEFAULT => -10
	},
	{#State 9
		ACTIONS => {
			'END_CT' => 17,
			'DIRECTION_TAG' => -5,
			'error' => 18
		},
		GOTOS => {
			'END' => 19
		}
	},
	{#State 10
		ACTIONS => {
			'DIRECTION_TAG' => 20,
			'error' => 21
		},
		GOTOS => {
			'DIRECTION' => 22
		}
	},
	{#State 11
		ACTIONS => {
			'error' => 24,
			'LEMMA' => 25
		},
		GOTOS => {
			'LEMMA_FIELD' => 23
		}
	},
	{#State 12
		DEFAULT => -12
	},
	{#State 13
		ACTIONS => {
			"\nPOSTAG: " => 26
		}
	},
	{#State 14
		DEFAULT => -11
	},
	{#State 15
		DEFAULT => -6
	},
	{#State 16
		DEFAULT => -9
	},
	{#State 17
		DEFAULT => -18
	},
	{#State 18
		ACTIONS => {
			"\nEND: " => 27
		}
	},
	{#State 19
		DEFAULT => -3
	},
	{#State 20
		DEFAULT => -20
	},
	{#State 21
		ACTIONS => {
			"\nDIRECTION: " => 28
		}
	},
	{#State 22
		ACTIONS => {
			'TAG' => -7,
			'error' => 6,
			'OPEN_CT' => 8
		},
		GOTOS => {
			'OPEN' => 2,
			'word' => 5,
			'@1-0' => 4,
			'gnm' => 29
		}
	},
	{#State 23
		ACTIONS => {
			'INFLFORM' => 32,
			'error' => 31
		},
		GOTOS => {
			'INFLFORM_FIELD' => 30
		}
	},
	{#State 24
		ACTIONS => {
			"\nLEMMA: " => 33
		}
	},
	{#State 25
		DEFAULT => -14
	},
	{#State 26
		DEFAULT => -13
	},
	{#State 27
		DEFAULT => -19
	},
	{#State 28
		DEFAULT => -21
	},
	{#State 29
		ACTIONS => {
			'END_CT' => 17,
			'error' => 18
		},
		GOTOS => {
			'END' => 34
		}
	},
	{#State 30
		DEFAULT => -8
	},
	{#State 31
		ACTIONS => {
			"\nINFLFORM_FIELD: " => 35
		}
	},
	{#State 32
		DEFAULT => -16
	},
	{#State 33
		DEFAULT => -15
	},
	{#State 34
		DEFAULT => -4
	},
	{#State 35
		DEFAULT => -17
	}
],
                                  yyrules  =>
[
	[#Rule 0
		 '$start', 2, undef
	],
	[#Rule 1
		 'line', 0, undef
	],
	[#Rule 2
		 'line', 2,
sub
#line 31 "GNMParser.yp"
{$$if_gnm_ref = $if_word;}
	],
	[#Rule 3
		 'gnm', 3,
sub
#line 34 "GNMParser.yp"
{ $if_word = pop @if_stack; }
	],
	[#Rule 4
		 'gnm', 5,
sub
#line 35 "GNMParser.yp"
{
	 $word_in_l = pop @if_stack;
	 my $if_l = pop @if_stack;
	 my $lm_l = pop @lm_stack;
	 $dir = pop @if_stack;
	 $word_in_r = pop @if_stack;
	 my $if_r = pop @if_stack;
	 my $lm_r = pop @lm_stack;
# 	 print STDERR "$if_l / $if_r\n";
#  	 print STDERR "(1)$lm_l / $lm_r\n";
# 	 print STDERR "$first_word / $last_word\n";
#  	 print STDERR "(2)$first_word / $last_word\n";
# 	 print STDERR "$word_in_l / $word_in_r\n";
	 if ($word_in_l == 2) {
	     $if_word = $if_l;
	     $lm_word = $lm_l;
	     push @if_stack, $if_l;
	     push @lm_stack, $lm_l;
	     push @if_stack, 2;
	 } else {
	     if ($word_in_r == 2) {
		 $if_word = $if_r;
		 $lm_word = $lm_r;
		 push @if_stack, $if_r;
		 push @lm_stack, $lm_r;
		 push @if_stack, 2;
	     } else {
		 if ($dir == -1 ) {
		     $if_word = "$if_l $if_r";
		     $lm_word = "$lm_l $lm_r";
		 } else {
		     if ($dir eq "") {
			 $if_word = "$if_r $if_l";
			 $lm_word = "$lm_r $lm_l";
		     } else {
			 $if_word = "$if_r $dir $if_l";
			 $lm_word = "$lm_r $lm_l";
		     }
		 }
		 $dir = 0;
		 if ($debug != 0) {print STDERR "+++> $if_word\n"; }
		 
		 $word_in = $word_in_l + $word_in_r;
		 if ($lm_l eq $first_word) { $word_in++;};
		 if ($lm_r eq $last_word) { $word_in++;};
		 if ($lm_r eq $first_word) { $word_in++;};
		 if ($lm_l eq $last_word) { $word_in++;};
# 		 print STDERR "Add in the stack ($word_in)\n";
# 		 print STDERR "Add in the stack ($word_in)\n";
# 		 print STDERR "$word_in_l / $word_in_r\n";
		 push @if_stack, $if_word;
		 push @if_stack, $word_in;
		 push @lm_stack, $lm_word;
	     }
	 }
     }
	],
	[#Rule 5
		 'gnm', 1,
sub
#line 91 "GNMParser.yp"
{
# 	 if ($if_stack[$#if_stack] eq "$last_word") {  $out_tc = 1;}
     }
	],
	[#Rule 6
		 'gnm', 2,
sub
#line 94 "GNMParser.yp"
{ $_[0]->YYErrok }
	],
	[#Rule 7
		 '@1-0', 0,
sub
#line 98 "GNMParser.yp"
{if ($debug != 0) {print STDERR "WORD BEGINS\n";}}
	],
	[#Rule 8
		 'word', 4,
sub
#line 98 "GNMParser.yp"
{if ($debug != 0) {print STDERR "WORD ENDS\n";}}
	],
	[#Rule 9
		 'word', 2,
sub
#line 99 "GNMParser.yp"
{ $_[0]->YYErrok }
	],
	[#Rule 10
		 'OPEN', 1,
sub
#line 102 "GNMParser.yp"
{if ($debug != 0) {print STDERR "OPEN_CT $_[1]\n";}}
	],
	[#Rule 11
		 'OPEN', 2,
sub
#line 103 "GNMParser.yp"
{ $_[0]->YYErrok }
	],
	[#Rule 12
		 'POSTAG', 1,
sub
#line 106 "GNMParser.yp"
{if ($debug != 0) {print STDERR "POSTAG $_[1]\n";}}
	],
	[#Rule 13
		 'POSTAG', 2,
sub
#line 107 "GNMParser.yp"
{ $_[0]->YYErrok }
	],
	[#Rule 14
		 'LEMMA_FIELD', 1,
sub
#line 110 "GNMParser.yp"
{
    if ($debug != 0) {print STDERR "LEMMA $_[1]\n";}
    push @lm_stack, $_[1];
    
    }
	],
	[#Rule 15
		 'LEMMA_FIELD', 2,
sub
#line 115 "GNMParser.yp"
{ $_[0]->YYErrok }
	],
	[#Rule 16
		 'INFLFORM_FIELD', 1,
sub
#line 119 "GNMParser.yp"
{
    if ($debug != 0) {print STDERR "INFLFORM $_[1]\n";} 
    push @if_stack, $_[1];
    push @if_stack, 0;
    }
	],
	[#Rule 17
		 'INFLFORM_FIELD', 2,
sub
#line 124 "GNMParser.yp"
{ $_[0]->YYErrok }
	],
	[#Rule 18
		 'END', 1,
sub
#line 127 "GNMParser.yp"
{if ($debug != 0) {print STDERR "END_CT $_[1]\n";}}
	],
	[#Rule 19
		 'END', 2,
sub
#line 128 "GNMParser.yp"
{ $_[0]->YYErrok }
	],
	[#Rule 20
		 'DIRECTION', 1,
sub
#line 131 "GNMParser.yp"
{
    if ($debug != 0) {print STDERR "DIRECTION_TAG $_[1]\n";}
    my $dir_tag = $_[1];
    if ($dir_tag eq "|-|") {
	$dir=-1;
    } else {
# 	if ($dir eq "|+|") {
	    $dir="";
# 	}
    }
    if ($dir != -1) {
	while ($dir_tag =~ /\[([^ ]+) \]/gc) {
	    $dir .= "$1 ";
	}
	chop $dir;
    }
    if ($debug != 0) {print STDERR "DIR = $dir\n";}
    push @if_stack, $dir;
    

}
	],
	[#Rule 21
		 'DIRECTION', 2,
sub
#line 152 "GNMParser.yp"
{ $_[0]->YYErrok }
	]
],
                                  @_);
    bless($self,$class);
}

#line 155 "GNMParser.yp"




sub _Error {
        exists $_[0]->YYData->{ERRMSG}
    and do {
        print STDERR $_[0]->YYData->{ERRMSG};
        delete $_[0]->YYData->{ERRMSG};
        return;
    };
    print STDERR "Syntax error.\n";
	exit;
}

sub _Lexer {
    my($parser)=shift;

    my $open = '(\{)';
    my $end = '(\})';
    my $direction = '(\|((\-)|(\+)|((<[^>]+>[^\[]+\[[^\]]+\])))+\|)';
    my $openIF = '(\[)';
    my $endIF = '(\])\t+';
    my $lemma = '([^\[\]]+) ';
    my $tag = '<[^>]+>';
    my $inflform = '\[([^\]]+) \]';

    
#     print STDERR $parser->YYData->{GNM} . "\n";

    $if_gnm_ref = $parser->YYData->{GNM_INF};

    $first_word = $parser->YYData->{FIRST_WORD} ;
    $last_word = $parser->YYData->{LAST_WORD};

    for ($parser->YYData->{GNM}) {
        s/^$open\s*// and return ('OPEN_CT', $1);
	s/^$tag// and return('TAG', $1);
	s/^$end// and return('END_CT', $1);
	s/^$direction// and return('DIRECTION_TAG', $1);
	s/^$inflform// and return('INFLFORM', $1);
	s/^$lemma// and return('LEMMA', $1);
    }

}

1;
