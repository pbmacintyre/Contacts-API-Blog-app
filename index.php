<?php
/**
 * Copyright (C) 2019-2024 Paladin Business Solutions
 */
ob_start();
session_start();

require_once('includes/ringcentral-functions.inc');
require_once('includes/ringcentral-php-functions.inc');

//show_errors();

function show_form($message, $print_again = false) {
    page_header(); ?>
    <form action="" method="post" enctype="multipart/form-data">
        <table class="EditTable">
            <tr class="CustomTable">
                <td colspan="2" class="CustomTableFullCol">
                    <img src="images/rc-logo.png"/>
                    <h2><?php echo app_name(); ?></h2>
                    <?php
                    if ($print_again) {
                        echo "<p class='msg_bad'>" . $message . "</p>";
                    } else {
                        echo "<p class='msg_good'>" . $message . "</p>";
                    } ?>
                    <hr>
                </td>
            </tr>
            <tr class="CustomTable">
                <td class="left_col">
                    <p style='display: inline;'>Starts with:</p>
                </td>
                <td class="right_col">
                    <input type="text" name="startsWith"><br/>
                    If specified, only contacts which 'First name' or 'Last name' start
                    with the mentioned substring will be returned. Case-insensitive
                </td>
            </tr>
            <tr class="CustomTable">
                <td class="left_col">
                    <p style='display: inline;'>Sort Results By:</p>
                </td>
                <td class="right_col">
                    <select name="sortBy">
                        <option value="" selected>No Sorting</option>
                        <option value="FirstName">First Name</option>
                        <option value="LastName">Last Name</option>
                        <option value="Company">Company</option>
                    </select>
                </td>
            </tr>
            <tr class="CustomTable">
                <td class="CustomTableFullCol">
                    <br/>
                    <input type="submit" class="submit_button" value="   Retrieve Company Contacts   "
                           name="get_company"/>
                </td>
                <td class="CustomTableFullCol">
                    <br/>
                    <input type="submit" class="submit_button" value="   Retrieve Guest Contacts   " name="get_guests">
                </td>
            </tr>
            <tr class="CustomTable">
                <td colspan="2" class="CustomTableFullCol">
                    <hr>
                </td>
            </tr>
        </table>
    </form>
    <?php
}

function check_form($list) {
    $print_again = false;
    $message = "";

    $sortBy = strip_tags($_POST['sortBy']);
    $startsWith = strip_tags($_POST['startsWith']);
    if ($list == "Company") {
        get_company_contacts($startsWith, $sortBy);
    }
    if ($list == "Guests") {
        get_guest_contacts($startsWith, $sortBy);
    }
}

/* ============= */
/*  --- MAIN --- */
/* ============= */
if (isset($_POST['get_company'])) {
    check_form("Company");
} elseif (isset($_POST['get_guests'])) {
    check_form("Guests");
} else {
    $message = "Please provide the needed information.";
    show_form($message);
}

ob_end_flush();
page_footer();
