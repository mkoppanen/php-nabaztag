--TEST--
Check for nabaztag presence
--SKIPIF--
<?php if (!extension_loaded("nabaztag")) print "skip"; ?>
--FILE--
<?php 
echo "nabaztag extension is available";
?>
--EXPECT--
nabaztag extension is available
