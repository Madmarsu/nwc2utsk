<?php

// -------------
class NWC2Clip {
// -------------

var $Version="";
var $Mode="";
var $Items=array();

function NWC2Clip($src='php://stdin')
{
	if (is_array($src)) $this->Items = $src;
	else if (!strstr($src,"\n")) $this->Items = gzfile($src);
	else if (($line = strtok($src,"\n")) !== false) {
		while ($line !== false) {
			$this->Items[] = $line."\n";
			$line = strtok("\n");
			}
		}

	if (!is_array($this->Items))
		trigger_error("Clip text from NoteWorthy Composer 2 is required as input to the NWC2Clip object",E_USER_ERROR);

	while (count($this->Items)) {
		$hdr = trim(array_shift($this->Items));

		if (NWC2ClassifyLine($hdr) == NWCTXTLTYP_FORMATHEADER) {
			if (preg_match('/^'.NWC2_STARTCLIP.'\(([0-9]+)\.([0-9]+)\,([a-zA-Z0-9_]+)/',$hdr,$m)) {
				$this->Version = "$m[1].$m[2]";
				$this->Mode = $m[3];
				break;
				}
			
			trigger_error("Unrecognized notation clip format header",E_USER_ERROR);
			}
		}

	if (!count($this->Items))
		trigger_error("Format error in the clip text",E_USER_ERROR);

	//
	$ftr = false;
	while (($ftr != NWC2_ENDCLIP) && (count($this->Items) > 0)) $ftr = trim(array_pop($this->Items));
	//
	if ($ftr != NWC2_ENDCLIP) trigger_error("Could not find clip ending tag",E_USER_ERROR);
}

function GetClipHeader()
{ 
	return NWC2_STARTCLIP."(".$this->Version.",".$this->Mode.")";
}

function GetClipFooter()
{ 
	return NWC2_ENDCLIP;
}
};

?>