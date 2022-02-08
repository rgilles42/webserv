<?php

var_dump($_POST);
var_dump($_GET);
var_dump($_ENV);
if (isset($_POST['montext'])) {
}

?>

<html>
 <head>
  <title>Test PHP</title>
 </head>
 <body>
 <?php echo '<p>Bonjour le monde</p>'; ?>

 <form method="get">
    <h1>Form 0</h1>
     <input type="text" name="montext">
     <input type="submit">
 </form>

 <form method="post" enctype="multipart/form-data">
    <h1>Form 1</h1>
     <input type="file" name="monficher">
     <input type="text" name="montext">
     <input type="submit">
 </form>
 <form method="post" enctype="application/x-www-form-urlencoded">
    <h1>Form 2</h1>
     <input type="file" name="monficher">
     <input type="text" name="montext">
     <input type="submit">
 </form>
 <form method="post" enctype="plain/text">
    <h1>Form 3</h1>
     <input type="file" name="monficher">
     <input type="text" name="montext">
     <input type="submit">
 </form>

 <form method="post" action="/debug_cgi/debug_cgi.cpp" enctype="multipart/form-data">
    <h1>Form 1 to debug</h1>
     <input type="file" name="monficher">
     <input type="text" name="montext">
     <input type="submit">
 </form>
 <form method="post" action="/debug_cgi/debug_cgi.cpp" enctype="application/x-www-form-urlencoded">
    <h1>Form 2 to debug</h1>
     <input type="file" name="monficher">
     <input type="text" name="montext">
     <input type="submit">
 </form>
 <form method="post" action="/debug_cgi/debug_cgi.cpp" enctype="plain/text">
    <h1>Form 3 to debug</h1>
     <input type="file" name="monficher">
     <input type="text" name="montext">
     <input type="submit">
 </form>
 </body>
</html>
