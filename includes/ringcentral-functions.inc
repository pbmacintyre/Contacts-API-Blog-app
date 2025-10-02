<?php
/** Copyright (C) 2019-2024 Paladin Business Solutions */

/* ================= */
/* Generic functions */
/* ================= */

function app_name() {
    return "Contacts Blog Demo App";
}

/* ================== */
/* Get RingCental SDK */
/* ================== */
function ringcentral_sdk() {
    // Include Libraries
    require('includes/vendor/autoload.php');

    $dotenv = Dotenv\Dotenv::createImmutable(__DIR__);
    $dotenv->load();

    $jwt_key = $_ENV['RC_JWT_KEY'];

    $sdk = new RingCentral\SDK\SDK(
            $_ENV['RC_APP_CLIENT_ID'],
            $_ENV['RC_APP_CLIENT_SECRET'],
            $_ENV['RC_SERVER_URL']);

    $platform = $sdk->platform();

    // Login via API
    if (!$sdk->platform()->loggedIn()) {
        try {
            $platform->login(["jwt" => $jwt_key]);
        } catch (\RingCentral\SDK\Http\ApiException $e) {
            $sdk = 0;
            // exit("<br/><br/>Unable to authenticate to platform. Check your RingCentral credentials. <br/><br/>") ;
        }
    }
    $controller = array('SDK' => $sdk, 'platform' => $platform);
    return $controller;
}

function get_company_contacts($startsWith, $sortBy) {
    $controller = ringcentral_sdk();
    $platform = $controller['platform'];

    $queryParams = "" ;
// array(
//		'startsWith' => $startsWith,
//		'sortBy' => $sortBy,
//		'sortBy' => "LastName",
//    );

    $endpoint = "/restapi/v1.0/account/~/directory/entries";

    try {
//        $resp = $platform->get($endpoint, $queryParams);
        $resp = $platform->get($endpoint);
//		echo_spaces("Response", $resp->json());
//		echo_spaces("Response record", $resp->json()->records);
        $records = $resp->json()->records; // array of objects
        if (count($records) >= 1) {
            usort($records, function($a, $b) {
                $lastA = $a->lastName ?? '';
                $lastB = $b->lastName ?? '';
                return strcmp($lastA, $lastB);
            });
            foreach ($records as $record) {
                // echo_spaces("Response record", $record);
//                if ($record->status == 'Enabled') {
                    echo_spaces("Contact ID", $record->id);
                    echo_spaces("Contact Status", $record->status);
                    echo_spaces("Contact name", $record->firstName . ' ' . $record->lastName);
//                    echo_spaces("Contact Last name", $record->lastName);
                    if (!$record->jobTitle) {
                        echo_spaces("No Job Title listed");
                    } else {
                        echo_spaces("Contact Job Title", $record->jobTitle);
                    }
                    echo_spaces("Contact account id", $record->account->id);
                    echo_spaces("Contact extension #", $record->extensionNumber);
                    if ($record->profileImage) {
                        echo_spaces("Contact Profile URI", $record->profileImage->uri, 2);

                        $img_account = $record->account->id;
                        $img_extension = $record->id;

                        $img_endpoint = "/restapi/v1.0/account/$img_account/extension/$img_extension/profile-image/90x90";
//                        $img_endpoint = "/restapi/v1.0/account/$img_account/extension/$img_extension/profile-image/195x195";
                        $img_resp = $platform->get($img_endpoint);

                        $contentType = $img_resp->response()->getHeaderLine("Content-Type");
                        $binary = $img_resp->raw();
                        $base64 = base64_encode($binary);
                        $imageSrc = "data:$contentType;base64,$base64"; ?>

                        <img src="<?php echo $imageSrc; ?>" alt="Profile">
                    <?php } else {
                        echo_spaces("Contact profile image not found.", "",2);;
                    }


//                }
            }
        } else {
            echo_spaces("No contact records found for provided parameters.");
        }
    } catch (\RingCentral\SDK\Http\ApiException $e) {
        // craft a friendly error message here.
        echo_spaces("There was an error retrieving the contacts list, Please try again later", $e->getMessage());
    }
}

function get_guest_contacts($startsWith, $sortBy) {
    $controller = ringcentral_sdk();
    $platform = $controller['platform'];

    $queryParams = array(
//		'startsWith' => 'A',
		'sortBy' => 'LastName',
    );

    $endpoint = "/restapi/v1.0/account/~/extension/~/address-book/contact";

    try {
        $resp = $platform->get($endpoint, $queryParams);
//        $resp = $platform->get($endpoint);
		echo_spaces("Response", $resp->json());
//		echo_spaces("Response record", $resp->json()->records);
        $records = $resp->json()->records; // array of objects
//        if (count($records) >= 1) {
//            usort($records, function($a, $b) {
//                $lastA = $a->lastName ?? '';
//                $lastB = $b->lastName ?? '';
//                return strcmp($lastA, $lastB);
//            });
//            foreach ($records as $record) {
//                // echo_spaces("Response record", $record);
//                if ($record->status == 'Enabled') {
//                    echo_spaces("Contact ID", $record->id);
//                    echo_spaces("Contact Status", $record->status);
//                    echo_spaces("Contact name", $record->firstName . ' ' . $record->lastName);
////                    echo_spaces("Contact Last name", $record->lastName);
//                    if (!$record->jobTitle) {
//                        echo_spaces("No Job Title listed");
//                    } else {
//                        echo_spaces("Contact Job Title", $record->jobTitle);
//                    }
//                    echo_spaces("Contact account id", $record->account->id);
//                    echo_spaces("Contact extension #", $record->extensionNumber);
//                    if ($record->profileImage) {
//                        echo_spaces("Contact Profile URI", $record->profileImage->uri, 2);
//
//                        $img_account = $record->account->id;
//                        $img_extension = $record->id;
//
//                        $img_endpoint = "/restapi/v1.0/account/$img_account/extension/$img_extension/profile-image/90x90";
////                        $img_endpoint = "/restapi/v1.0/account/$img_account/extension/$img_extension/profile-image/195x195";
//                        $img_resp = $platform->get($img_endpoint);
//
//                        $contentType = $img_resp->response()->getHeaderLine("Content-Type");
//                        $binary = $img_resp->raw();
//                        $base64 = base64_encode($binary);
//                        $imageSrc = "data:$contentType;base64,$base64"; ?>
<!---->
<!--                        <img src="--><?php //echo $imageSrc; ?><!--" alt="Profile">-->
<!--                    --><?php //} else {
//                        echo_spaces("Contact profile image not found.", "",2);;
//                    }
//
//
//                }
//            }
//        } else {
//            echo_spaces("No contact records found for provided parameters.");
//        }
    } catch (\RingCentral\SDK\Http\ApiException $e) {
        // craft a friendly error message here.
        echo_spaces("There was an error retrieving the contacts list, Please try again later", $e->getMessage());
//        echo_spaces("details", $e);
    }
}