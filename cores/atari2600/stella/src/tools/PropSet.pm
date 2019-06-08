package PropSet;

# NOTE: If the property types ever change in Stella, the following hashmap
#       and array must be updated (and stay in sequence)
my %prop_type = (
  "Cartridge.MD5"           => 0,
  "Cartridge.Manufacturer"  => 1,
  "Cartridge.ModelNo"       => 2,
  "Cartridge.Name"          => 3,
  "Cartridge.Note"          => 4,
  "Cartridge.Rarity"        => 5,
  "Cartridge.Sound"         => 6,
  "Cartridge.Type"          => 7,
  "Console.LeftDifficulty"  => 8,
  "Console.RightDifficulty" => 9,
  "Console.TelevisionType"  => 10,
  "Console.SwapPorts"       => 11,
  "Controller.Left"         => 12,
  "Controller.Right"        => 13,
  "Controller.SwapPaddles"  => 14,
  "Controller.MouseAxis"    => 15,
  "Display.Format"          => 16,
  "Display.YStart"          => 17,
  "Display.Height"          => 18,
  "Display.Phosphor"        => 19,
  "Display.PPBlend"         => 20
);
my @prop_type_as_string = (
  "Cartridge.MD5",
  "Cartridge.Manufacturer",
  "Cartridge.ModelNo",
  "Cartridge.Name",
  "Cartridge.Note",
  "Cartridge.Rarity",
  "Cartridge.Sound",
  "Cartridge.Type",
  "Console.LeftDifficulty",
  "Console.RightDifficulty",
  "Console.TelevisionType",
  "Console.SwapPorts",
  "Controller.Left",
  "Controller.Right",
  "Controller.SwapPaddles",
  "Controller.MouseAxis",
  "Display.Format",
  "Display.YStart",
  "Display.Height",
  "Display.Phosphor",
  "Display.PPBlend"
);

my @prop_defaults = (
  "",
  "",
  "",
  "Untitled",
  "",
  "",
  "MONO",
  "AUTO",
  "B",
  "B",
  "COLOR",
  "NO",
  "JOYSTICK",
  "JOYSTICK",
  "NO",
  "AUTO",
  "AUTO",
  "34",
  "210",
  "NO",
  "77"
);

# Load and parse a properties file into an hash table of property
# objects, indexed by md5sum
sub load_prop_set($) {
  my $file = $_[0];
  print "Loading properties from file: $file\n";

  my @props = ();
  while(($key, $value) = each(%prop_type)) {
    $props[$value] = "";
  }

  my %propset = ();
  open(INFILE, $file);
  foreach $line (<INFILE>) {
    chomp $line;

    # Start a new item
    if ($line =~ /^""/) {
      my $key = $props[$prop_type{'Cartridge.MD5'}];
#     print "Inserting properties for key = $key\n";

      if(defined($propset{$key})) {
        print "Duplicate: $key\n";
      }
      $propset{$key} = [ @props ];

      undef @props;
      while(($key, $value) = each(%prop_type)) {
        $props[$value] = "";
      }
    } elsif ($line !~ /^$/) {
      ($key, $value) = ($line =~ m/"(.*)" "(.*)"/);
      if (defined $prop_type{$key}) {
        $index = $prop_type{$key};
        $props[$index] = $value;
      } else {
        print "ERROR: $line\n";
        print "Invalid key = \'$key\' for md5 = \'$props[0]\', ignoring ...\n";
      }
    }
  }
  close(INFILE);
  return %propset;
}

# Load and parse a properties file into an hash table of property
# objects, indexed by md5sum
sub save_prop_set {
  my $file = shift;
  my $hashref = shift;
  print "Saving " . keys(%$hashref) . " properties to file: $file\n";

  open(OUTFILE, ">$file");
  while (($md5, $props) = each(%$hashref)) {
    my @array = @$props;
    for (my $i = 0; $i < @array; $i++) {
      if ($array[$i] ne "") {
        print OUTFILE "\"$prop_type_as_string[$i]\" \"$array[$i]\"\n";
      }
    }
    print OUTFILE "\"\"\n\n";
  }
  close(OUTFILE);
}

# Get the number of property tags in one PropSet element
sub num_prop_types {
  return keys( %prop_type );
}

# Convert a properties set into a C++ compatible string
sub build_prop_string {
  my @array = @_;
  my $result = "  { ";
  my @items = ();
  for (my $i = 0; $i < @array; $i++) {
    if($prop_defaults[$i] ne $array[$i]) {
      push(@items, "\"$array[$i]\"");
    } else {
      push(@items, "\"\"");
    }
  }

  $result .= join(", ", @items);
  $result .= " }";

  return $result;
}
