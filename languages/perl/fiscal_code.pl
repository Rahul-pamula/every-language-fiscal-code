#!/usr/bin/env perl
# Italian Fiscal Code (codice fiscale) — Perl implementation.
#
# Reads one JSON object from stdin and writes the result to stdout,
# following SPECIFICATION.md. Uses only core modules (JSON::PP).

use strict;
use warnings;
use JSON::PP;

my $MONTH_CODES = 'ABCDEHLMPRST';
my @FIELDS = qw(surname name birth_date sex birthplace_code);
my %ODD_VALUES;
@ODD_VALUES{ 0 .. 9, 'A' .. 'Z' } = (
    1, 0, 5, 7, 9, 13, 15, 17, 19, 21,
    1, 0, 5, 7, 9, 13, 15, 17, 19, 21, 2, 4, 18, 20, 11, 3,
    6, 8, 12, 14, 16, 10, 22, 25, 24, 23,
);

# Plain (non-reference) defined scalar. Numbers never satisfy the field
# formats below, so they need not be told apart from strings.
sub is_text { my ($v) = @_; return defined $v && !ref $v }

sub normalize_name {
    my ($value, $code) = @_;
    die { error => $code } unless is_text($value) && $value =~ /\A[A-Za-z '\-]*\z/;
    (my $letters = uc $value) =~ s/[^A-Z]//g;
    die { error => $code } if $letters eq '';
    return $letters;
}

sub encode_part {
    my ($text, $is_name) = @_;
    (my $consonants = $text) =~ s/[AEIOU]//g;
    (my $vowels = $text) =~ s/[^AEIOU]//g;
    if ($is_name && length $consonants >= 4) {
        return join '', map { substr $consonants, $_, 1 } 0, 2, 3;
    }
    return substr $consonants . $vowels . 'XXX', 0, 3;
}

sub parse_date {
    my ($value) = @_;
    die { error => 'invalid_birth_date' }
        unless is_text($value) && $value =~ /\A([0-9]{4})-([0-9]{2})-([0-9]{2})\z/;
    my ($year, $month, $day) = ($1 + 0, $2 + 0, $3 + 0);
    my $leap = ($year % 4 == 0 && $year % 100 != 0) || $year % 400 == 0;
    my @days = (31, $leap ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
    die { error => 'invalid_birth_date' }
        if $year < 1 || $month < 1 || $month > 12 || $day < 1 || $day > $days[$month - 1];
    return ($year, $month, $day);
}

sub check_character {
    my ($partial) = @_;
    my $sum = 0;
    my @chars = split //, $partial;
    for my $i (0 .. $#chars) {
        my $c = $chars[$i];
        if ($i % 2 == 0) { $sum += $ODD_VALUES{$c} }
        elsif ($c =~ /[0-9]/) { $sum += $c }
        else { $sum += ord($c) - ord('A') }
    }
    return chr(ord('A') + $sum % 26);
}

sub fiscal_code {
    my ($input) = @_;
    die { error => 'invalid_json' } unless ref $input eq 'HASH';
    for my $field (@FIELDS) {
        die { error => 'missing_field' } unless exists $input->{$field};
    }
    my $surname = normalize_name($input->{surname}, 'invalid_surname');
    my $name = normalize_name($input->{name}, 'invalid_name');
    my ($year, $month, $day) = parse_date($input->{birth_date});
    my $sex = $input->{sex};
    die { error => 'invalid_sex' } unless is_text($sex) && ($sex eq 'M' || $sex eq 'F');
    my $place = $input->{birthplace_code};
    die { error => 'invalid_birthplace_code' }
        unless is_text($place) && $place =~ /\A[A-Z][0-9]{3}\z/;

    my $partial = sprintf '%s%s%02d%s%02d%s', encode_part($surname, 0), encode_part($name, 1),
        $year % 100, substr($MONTH_CODES, $month - 1, 1), $sex eq 'F' ? $day + 40 : $day, $place;
    return $partial . check_character($partial);
}

my $json = JSON::PP->new->utf8->allow_nonref->canonical;
my $raw = do { local $/; <STDIN> };
my $result = eval {
    my $input = eval { $json->decode($raw) };
    die { error => 'invalid_json' } if $@;
    { fiscal_code => fiscal_code($input) };
};
if (!$result) {
    die $@ unless ref $@ eq 'HASH';
    print $json->encode($@), "\n";
    exit 1;
}
print $json->encode($result), "\n";
exit 0;
