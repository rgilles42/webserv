<html>
<head>
   <title>Test PHP</title>
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
      <h1>FORMULAIRE HTML</h1>
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


   <form action="succesful.html" method="POST" enctype="multipart/form-data">
         Select text file to upload:
         <input type="file" name="fileToUpload" id="fileToUpload">
         <input type="submit" value="submit" name="submit">
   </form>

   curl -v -X DELETE 127.0.0.1:8080/uploads/rush03.c
</body>
</html>
