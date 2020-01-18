<?php
if ($_GET['action'] === "source"){
    highlight_file(__FILE__);
    die();
}
?>
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>Option-Cmd-U</title>
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/bulma/0.7.5/css/bulma.min.css">
        <script defer src="https://use.fontawesome.com/releases/v5.3.1/js/all.js"></script>
    </head>
    <body>
        <div class="container">                
            <section class="hero">
                <div class="hero-body">
                    <div class="container">
                        <h1 class="title has-text-centered has-text-weight-bold">
                            Option-Cmd-U
                        </h1>
                        <h2 class="subtitle has-text-centered">
                            "View Page Source" is no longer required! Let's view page source online :-)
                        </h2>
                        <form method="GET">
                            <div class="field has-addons">
                                <div class="control is-expanded">
                                    <input class="input" type="text" placeholder="URL (e.g. http://example.com)" name="url" value="<?= htmlspecialchars($_GET['url'], ENT_QUOTES, 'UTF-8') ?>">
                                </div>
                                <div class="control">
                                    <button class="button is-link">Submit</button>
                                </div>
                            </div>
                        </form>
                    </div>
                </div>
            </section>
            <section class="section">
                <pre>
                    <!-- src of this PHP script: /index.php?action=source -->
                    <!-- the flag is in /flag.php, which permits access only from internal network :-) -->
                    <!-- this service is running on php-fpm and nginx. see /docker-compose.yml -->
                    <?php
                    if (isset($_GET['url'])){
                        $url = filter_input(INPUT_GET, 'url');
                        $parsed_url = parse_url($url);                        
                        if($parsed_url["scheme"] !== "http"){
                            // only http: should be allowed. 
                            echo 'URL should start with http!';
                        } else if (gethostbyname(idn_to_ascii($parsed_url["host"], 0, INTL_IDNA_VARIANT_UTS46)) === gethostbyname("nginx")) {
                            // local access to nginx from php-fpm should be blocked.
                            echo 'Oops, are you a robot or an attacker?';
                        } else {
                            // file_get_contents needs idn_to_ascii(): https://stackoverflow.com/questions/40663425/
                            highlight_string(file_get_contents(idn_to_ascii($url, 0, INTL_IDNA_VARIANT_UTS46),
                                                               false,
                                                               stream_context_create(array(
                                                                   'http' => array(
                                                                       'follow_location' => false,
                                                                       'timeout' => 2
                                                                   )
                                                               ))));
                        }
                    }
                    ?>
                </pre>
            </section>
        </div>            
    </body>
</html>
