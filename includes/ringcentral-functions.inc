<?php
/** Copyright (C) 2019-2024 Paladin Business Solutions */

/* ================= */
/* Generic functions */
/* ================= */

function app_name() {
	return "List Call Log Blog Demo App";
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

function get_logs($log_number, $direction, $call_type, $call_details, $startDate, $endDate) {
	$controller = ringcentral_sdk();
	$platform = $controller['platform'];

	$startDate = date('Y-m-d\TH:i:s\Z', strtotime($startDate));
	$endDate = date('Y-m-d\TH:i:s\Z', strtotime($endDate));

	$queryParams = array(
		'dateFrom' => $startDate,
		'dateTo' => $endDate,
		'phoneNumber' => $log_number,
		'view' => $call_details,
		'direction' => $direction,
		'type' => $call_type
	);
//	echo_spaces("Query Params", $queryParams);
	$endpoint = "/restapi/v1.0/account/~/extension/~/call-log";

	try {
		$resp = $platform->get($endpoint, $queryParams);
//		echo_spaces("Response", $resp->json());
//		echo_spaces("Response record", $resp->json()->records);
		if (count($resp->json()->records) >= 1) {
			foreach ($resp->json()->records as $record) {
				if ($record->duration > 300) {
					// duration is in seconds 300 = 5 minutes
					echo_spaces("Call Start Date", date("Y-M-d", strtotime($record->startTime)));
					echo_spaces("Call Type", $record->type);
					echo_spaces("Call From", $record->from->name);
					echo_spaces("Call From #", $record->from->phoneNumber);
					echo_spaces("Call From Location", $record->to->location, 2);
				} else {
					echo_spaces("Response record", $record);
				}
			}
		} else {
			echo_spaces("No log records found for provided parameters.");
		}
	} catch (\RingCentral\SDK\Http\ApiException $e) {
		// craft a friendly error message here.
		echo_spaces("There was an error retrieving the call logs, Please try again later", $e->getMessage());
	}
}