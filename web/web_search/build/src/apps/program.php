<?php

$db_host = $config_data['DB_SECTION']['DB_HOST'];
$db_name = $config_data['DB_SECTION']['DB_NAME'];
$db_user = $config_data['DB_SECTION']['DB_USER'];
$db_pass = $config_data['DB_SECTION']['DB_PASS'];

$limit = intval($config_data['APP_SECTION']['NUM_PER_PAGE']);
$page = isset($_GET['page']) && ctype_digit($_GET['page']) ? intval($_GET['page']) - 1 : 0;
$page = $page < 0 ? 0 : $page;
$search_word = isset($_GET['q']) ? $_GET['q'] : null;
$offset = $limit * $page;

if (!is_null($search_word)) {
    $search_word = str_ireplace(array(' ', '%', '|', '!', ',', 'AND', 'OR'), '', $search_word);
    if (strlen($search_word) === 0) {
        $search_word = null;
    }
}

$query_parameters['page'] = $page + 1;
$query_parameters['q'] = $search_word;

try {
    $dbh = new PDO(sprintf('mysql:dbname=%s;host=%s;charset=utf8mb4', $db_name, $db_host),  $db_user, $db_pass);
    $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    $dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);

    $sql  = "";
    $sql .= " SELECT title, description, reference FROM articles WHERE status = 0 ";
    if (!is_null($search_word)) {
        $sql .= " AND description LIKE '%" . $search_word . "%' ";
    }
    $sql .= " ORDER BY id ASC LIMIT :offset, :limit ";

    $stmt = $dbh->prepare($sql);
    $stmt->bindParam(':offset', $offset, PDO::PARAM_INT);
    $stmt->bindParam(':limit', $limit, PDO::PARAM_INT);
    $stmt->execute();
    include 'view_head.php';

    $num_result = 0;
    echo '<dl>';
    while ($row = $stmt->fetch()) {
        echo sprintf("<dt>%s</dt>", $row['title']);
        echo sprintf("<dd>%s</dd>", $row['description']);
        $num_result++;
    }
    echo '</dl>';
    if ($num_result === 0) {
        echo "No result";
    }

    $query_paramegers_page_prev = $query_parameters;
    $query_paramegers_page_prev['page'] = $page;

    $query_paramegers_page_next = $query_parameters;
    $query_paramegers_page_next['page'] = ($num_result === $limit) ? $page + 2 : $page + 1;

    include 'view_bottom.php';

} catch (PDOException $e) {
    // echo $e->getMessage();
    include 'view_head.php';
    echo '<dl></dl>';
    echo "Error";
    include 'view_bottom.php';
}
