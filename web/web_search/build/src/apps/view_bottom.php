
<p>
    <?php if ($page === 0): ?>
        Prev
    <?php else: ?>
        <a href="./?<?php echo http_build_query($query_paramegers_page_prev); ?>">Prev</a>
    <?php endif; ?>
    /
    <?php if ($num_result === $limit): ?>
        <a href="./?<?php echo http_build_query($query_paramegers_page_next); ?>">Next</a>
    <?php else: ?>
        Next
    <?php endif; ?>
</p>

</body>
</html>
