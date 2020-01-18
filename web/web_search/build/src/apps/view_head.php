<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Articles</title>
</head>
<body>

<form action="./" method="get">
    <input type="text" name="q" value="<?php echo htmlspecialchars($search_word, ENT_QUOTES) ?>"><input type="submit" value="Search">
</form>
