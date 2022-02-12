<html>
<head>
   <title>PHP Test</title>
</head>
<body>
   <?php echo '<p>Hello everyone, this page was processed by PHP on ' . date('l jS \of F Y h:i:s A') . '</p>'; ?>

   <form method="get">
      <h1>GET TEST</h1>
      <p> The current get parameter is <b>"<?php if(isset($_GET['mytext'])) {echo $_GET["mytext"];}?>"</b><p>
      <input type="text" name="mytext">
      <input type="submit">
   </form>

   <form class="" method="POST">
      <h1>POST FORM TEST</h1>
      <div>
         The current user is:
         <br>
         <b>First name: "<?php if(isset($_POST['FirstName'])) {echo $_POST["FirstName"];}?>"</b>
         <br>
         <b>Last name: "<?php if(isset($_POST['LastName'])) {echo $_POST["LastName"];}?>"</b>
         <br>
         <b>Email: "<?php if(isset($_POST['Email'])) {echo $_POST["Email"];}?>"</b>
      </div>
      <div class="lab">
         <label for="FirstName">First Name: </label>
         <input name="FirstName" id="FirstName" value="">
         <br>
         <label for="LastName">Last Name: </label>
         <input name="LastName" id="LastName" value="">
         <br>
         <label for="Email">Email: </label>
         <input name="Email" id="Email" value="">
         <br>
         <button class="button buttonGet">Send</button>
      </div>
   </form>

   <form action="" method="POST" enctype="multipart/form-data">
      <h1>PHP UPLOAD TEST</h1>
      <div>
         Uploaded file info: <?php if(isset($_FILES['fileToUpload'])) {var_dump($_FILES["fileToUpload"]);}?>
      </div>
      <div>
         <p>Select text file to upload to PHP:</p>
         <input type="file" name="fileToUpload" id="fileToUpload">
         <input type="submit" value="submit" name="submit">
      </div>
   </form>

   <form action="../debug_cgi/test.debug" method="POST" enctype="multipart/form-data">
      <h1>DEBUG_CGI UPLOAD TEST</h1>
      <div>
         <p>Select text file to upload to Debug_CGI:</p>
         <input type="file" name="fileToUpload" id="fileToUpload">
         <input type="submit" value="submit" name="submit">
      </div>
   </form>
</body>
</html>
