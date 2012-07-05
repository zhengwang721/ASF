/* Options */
display_only_quickstart = false;
display_only_application_notes = false;
display_selection_menu = true;
/* This define will display only a certain type of item */
display_only_type = null;
/* This define will display only a certain ASF type of item */
display_only_asf_type = null;
/* This define will display only a certain category */
display_only_category = null;
/* Display only modules which matches this filter */
display_regexpr_filter = null;
/* Display specific module IDs */
display_ids = new Array();
/* Display columns */
display_column_type = true;
display_column_category = true;
display_column_device = true;
display_column_quickstart = false;
display_column_appnote = false;
display_column_board = false;

// This table contains all the itme of the main list
var doxygen_item_list = new Array();
// Contains the list of supported devices
var supported_devices = new Array();
// Contains the list of supported boards
var supported_boards = new Array();
// Selector of the table
var jq_table_selector = "#id_table";
// Selector of the category combobox
var jq_category_selector = "#navigation .category select";
// Selector of the type combobox
var jq_type_selector = "#navigation .type select";
// Selector of the mcu combobox
var jq_mcu_selector = "#navigation .device select";
// Selector of the kit combobox
var jq_kit_selector = "#navigation .kit select";
// Selector of the searchbox
var jq_search_selector = "#MSearchField";
// Selector of the header
var jq_head_selector = "#header";
// Selector of the navigation
var jq_nav_selector = "#navigation";
// Selector of the body
var jq_body_selector = "#body";
// Selector of the description
var jq_desc_selector = "#id_description";
// Default sorting method
var sorting_method = [[1, 0], [2, 0]];
/* String used to speed up the filtering */
var search_string = "";
/* Table used tospeed up a search with device name */
var mcu_search_table = new Array();

function display_none()
{
	display_only_quickstart = false;
	display_only_application_notes = false;
	display_column_type = false;
	display_column_category = false;
	display_column_device = false;
	display_column_quickstart = false;
	display_column_appnote = false;
        display_column_board = false;
}

function load_bugzilla_feed(url, container, nb_entries, add_link_to_bugzilla_list, callback)
{
	var feed_url = url;

	/* If a limit is set */
	if (typeof nb_entries != "undefined") {
		feed_url += "&limit=" + nb_entries;
	}

	/* By default do not add a link to the bugzilla */
	if (typeof add_link_to_bugzilla_list == "undefined") {
		add_link_to_bugzilla_list = false;
	}

	/* Create the callback */
	load_bugzilla_feed.callback = function(is_success, table_data) {
		$(container).removeClass("loading");
		table_print(table_data, container);
		if (typeof callback != "undefined") {
			callback(table_data);
		}
	};

	/* Format the output into a Atom feed */
	feed_url += "&ctype=atom";

	/* Trick to avoid the cache */
	feed_url += "&random_number=" + Math.random();

	/* Notify the user that someting is loading */
	$(container).addClass("loading");

	/* Use Google API to avoid the Cross-Origin Resource Sharing (CORS) issue */
	feed_url = 'http://ajax.googleapis.com/ajax/services/feed/load?v=1.0&num=10&callback=?&q=' + encodeURIComponent(feed_url);
	$.ajax({
		url: feed_url,
		dataType: 'json',
		success: function(data) {
			var table_data = new Array();
			if (typeof data != "undefined"
					&& typeof data.responseData != "undefined"
					&& typeof data.responseData.feed != "undefined"
					&& typeof data.responseData.feed.entries != "undefined") {
				for (var i in data.responseData.feed.entries) {
					var item = data.responseData.feed.entries[i];
					var bugzilla_status;
					$(item["content"]).find("td:nth-child(1)").each(function() {
						if ($(this).html().match(/status/i)) {
							bugzilla_status = $(this).next("td").html();
							return false;
						}
					});
					var bugzilla_name = item["title"].replace(/^\[[^\]]+\]/, "");
					table_data.push({
						'name': bugzilla_name,
						'url': item["link"],
						'class_attr': bugzilla_status,
						'new_page': true
					});
				}
			}
			/* Add a link to the bugzillalist if specified */
			if (add_link_to_bugzilla_list) {
				table_data.push({
					'name': "...",
					'url': url,
					'new_page': true
				});
			}
			load_bugzilla_feed.callback(true, table_data);
		},
		error: function() {
			var table_data = new Array();
			table_data.push({
				'name': "<i>Error: cannot load feed entries</i>",
				'url': null
			});
			load_bugzilla_feed.callback(false, table_data);
		}
	});
}

function mainpage_init()
{
	/* Hide the selection menu if specified */
	if (!display_selection_menu) {
		$("#navigation").hide();
	}

	/* If the category filter is on, make the re-search case insensitive */
	if (display_only_category) {
		display_only_category = display_only_category.toLowerCase();
	}

	/* Specific actions for different page types */
	for (var i = 0; i < asf_content.length;	i++) {
                display = true;
		/* Default status of this item, displayed or not... */
		if (display_only_quickstart || display_only_application_notes) {
			/* Is false if the user wants to display only quickstarts or ANs */
			display = false;
		}

		/* Add application note link */
		for (var j in asf_application_notes) {
			var rx = new RegExp("^" + asf_application_notes[j]['id_regexpr'] + "$");
			if (asf_content[i]["id"].match(rx)) {
				asf_content[i]["appnote"].push(asf_application_notes[j]);
			}
		}

		/* Check if this modules needs to be added to the list */
		if (display_only_quickstart) {
			for (arch in asf_content[i]["quickstart"]) {
				display = true;
				break;
			}
		}
                display |= (display_only_application_notes && asf_content[i]["appnote"].length);
		/* If only one item type is meant to be displayed */
		if (display && display_only_type) {
			display = (display_only_type && asf_content[i]["type"] == display_only_type);
		}
		/* If only one item of ASF type is meant to be displayed */
		if (display && display_only_asf_type) {
			display = (display_only_asf_type && asf_content[i]["asf_type"] == display_only_asf_type);
		}

		/* If only a certain category is meant to be displayed */
		if (display && display_only_category) {
			display &= (asf_content[i]["category"].join().toLowerCase().indexOf(display_only_category) != -1);
		}

		/* Apply the regular expression filter if any */
		if (display && display_regexpr_filter) {
			var rx = new RegExp(display_regexpr_filter);
			display &= (asf_content[i]["id"].match(rx) != null);
		}

		/* Check if the module ID is defined the specific ID list to be displayed */
		for (var j in display_ids) {
			if (asf_content[i]["id"] == display_ids[j]) {
				display = true;
				break;
			}
		}

		/* Add this item to the list if it is intended to be displayed */
		if (display) {
			table_add(asf_content[i]);
		}
	}

	/* Get the item_list */
	var item_list = get_item_list();

	/* Generate the search string */
        search_string = build_search_string_from_item_list(item_list);

	/* Generate the MCU search table */
        build_search_table_from_mcu_arch(asf_mcu_arch);

	/* Setup the different categories */
	category_init(item_list);
	type_init(item_list);
	mcus_init(item_list);
	kits_init(item_list);

	/* If there is a query string in the URL, parse it and select the categories */
        var q = $.parseQuery();
	if (typeof q.search == "string") {
		$(jq_search_selector).val(q.search);
	}
	if (typeof q.device == "string") {
		$(jq_mcu_selector).val(q.device);
	}
	if (typeof q.board == "string") {
		$(jq_kit_selector).val(q.board);
	}
	if (typeof q.type == "string") {
		$(jq_type_selector).val(q.type);
	}
	if (typeof q.category == "string") {
		$(jq_category_selector).val(q.category);
	}

	/* Update the table */
        table_filter();
}

function bugzilla_report(item, params)
{
	var url = "http://asf.atmel.com/bugzilla/enter_bug.cgi";
	var param_str = "";
	var component_values = {
		'unit test': "unit_*test",
		'3rd party': "^thirdparty\.",
		'avr32': "^avr32\.",
		'common': "^common\.",
		'mega': "^mega\.",
		'sam': "^sam\.",
		'xmega': "^xmega\.",
		'other': ".*"
	};

	/* Auto-fill the component value */
	for (var component in component_values) {
		var rx = new RegExp(component_values[component]);
		if (item["id"].match(rx)) {
			params["component"] = component;
			break;
		}
	}

	/* Auto-fill the product value */
	params["product"] = "Atmel Software Framework";

	/* Auto-fill the version value */
	var pattern = new RegExp("^([0-9]+)\.([0-9]+)");
	version_match = pattern.exec(asf_version);
	params["version"] = "v" + version_match[1] + "." + version_match[2] + ".x";

	for (var param_name in params) {
		if (param_str) {
			param_str += "&";
		}
		param_str += param_name + "=" + encodeURIComponent(params[param_name]);
	}

	window.open(url + "?" + param_str, '_blank');
}

function report_bug(index)
{
	var item = doxygen_item_list[index];
	bugzilla_report(item, {
		"short_desc": "",
		"comment": "--- Please keep this header ---\n"
				+ "ASF version: " + asf_version + "\n"
				+ "id: " + item["id"] + "\n"
				+ "--- Write a description of the bug here ----\n\n"
				+ "I would like to report a bug on this " + item["type"] + " (" + item["name"] + ").\n"
	});
}

function suggest_enhancement(index)
{
	var item = doxygen_item_list[index];
	bugzilla_report(item, {
		"short_desc": "",
		"comment": "--- Please keep this header ---\n"
				+ "ASF version: " + asf_version + "\n"
				+ "id: " + item["id"] + "\n"
				+ "--- Write a description of the enhancement here ----\n\n"
				+ "I have a sugggestion to improve this " + item["type"] + " (" + item["name"] + ").\n",
		"bug_severity": "enhancement"
	});
}

var search_in_button = false;
var search_in_select_window = false;
var search_timeout = null;
var search_page = "search.html";

function search_select(index)
{
	var search_page_list = [
		null,
		"search.html",
		"api.html",
		"applications.html",
		"unit_tests.html",
		"get_started.html"
	];
	/* Un-select the previous selection */
	$("#search_select_window a").children(".selection_mark").html("&nbsp;");
	/* Select the new item */
	$("#search_select_window a:nth-child(" + index + ")").children(".selection_mark").html("&#8226;");
	/* Select the search page */
	search_page = search_page_list[index - 1];
}

function search_init()
{
	/* Add an item to the list if the dynamic table is present
	 * and select the first one in the list
	 */
	if (!$(jq_table_selector).size()) {
		$("#search_select_window > a:first").hide();
		search_select(2);
	}
	else {
		search_select(1);
	}

	/* Function to hide the select window */
	search_init.hide = function () {
		if (!search_in_button && !search_in_select_window) {
			$("#search_select_window").hide();
		}
		search_timeout = null;
	};

	/* Initialize events attributed to the button and the select window */
	$("#search_select_window").hover(function() {
		search_in_select_window = true;
		if (search_timeout) {
			clearTimeout(search_timeout);
		}
	}, function() {
		search_in_select_window = false;
		search_timeout = setTimeout(search_init.hide, 100);
	});
	$("#MSearchBox .right").hover(function() {
		search_in_button = true;
		$("#search_select_window").show();
		if (search_timeout) {
			clearTimeout(search_timeout);
		}
	}, function() {
		search_in_button = false;
		search_timeout = setTimeout(search_init.hide, 100);
	});

	/* Trigger the re-search on 'enter' key-pressed */
	$(jq_search_selector).keypress(function(event) {
		if (event.keyCode == '13') {
			trigger_search(false);
		}
	});
}

function trigger_search(on_key_press)
{
	/* Check if there is a table on the page */
	if ($(jq_table_selector).size() && !search_page) {
		/* If so set a timeout to the search in order to make the research when the word as been typed */
		if (typeof trigger_search.h != "undefined" && trigger_search.h) {
			clearTimeout(trigger_search.h);
		}
		if (on_key_press) {
			trigger_search.h = setTimeout(function() { table_filter(); trigger_search.h = null; }, 500);
		}
		else {
                        table_filter();
		}
	}
	else {
		/* Go to the search page if the button search has been pressed */
		if (!on_key_press) {
			window.location = search_page + "?search=" + $(jq_search_selector).val();
		}
	}
}

function supported_boards_add(board_name)
{
	supported_boards.push(board_name);
}

function supported_devices_add(device_name, device_filter)
{
	var item = {
		"name": device_name,
		"filter": device_filter
	};
	supported_devices.push(item);
}

/* \brief Print a list of data in a table. It will create a list
 * following this pattern:
 * 1 4
 * 2 5
 * 3 6
 * \param table_data The list of element to be printed
 * \param container The identifier of the container where table should be printed, can be an ID or an object
 * \param nb_columns The number of columns for the table
 * \param sort Boolean which tells if the table needs to be sorted or not
 */
function table_print(table_data, container, nb_columns, sort)
{
	var column;
	var row;
	var row_index = new Array();
	var nb_rows;
	var body = "";

	/* By default the table will have only 1 column */
	if (typeof nb_columns == "undefined") {
		nb_columns = 1;
	}

	/* By default don't sort the table */
	if (typeof sort == "undefined") {
		sort = false;
	}

	/* Compute the number of rows */
        nb_rows = Math.ceil(table_data.length / nb_columns);

	/* Sort function */
	table_print.sort = function(a, b) {
		return a.name.toLowerCase().localeCompare(b.name.toLowerCase());
	}
	/* Sort the table */
	if (sort) {
		table_data = table_data.sort(table_print.sort);
	}

	/* Set the row index. This index will determine which item should be
	 * printed in a specific column.
	 */
	for (column = 0; column < nb_columns; column++) {
                row_index[column] = column * nb_rows;
	}

	/* Build the table */
	for (row = 0; row < nb_rows; row++) {
		body += "<tr>";
		/* Go through the columns */
		for (column = 0; column < nb_columns; column++) {
			/* Index of the element to be printed */
			var i = row_index[column]++;
			/* Check if an element needs to be printed in this column */
			if (typeof table_data[i] != "undefined") {
				var data = table_data[i];
				body += "<td class=\"table_print nowrap\">";
				if (typeof data.class_attr == "string") {
					body += "<p class=\"" + data.class_attr + "\">";
				}
				else {
					body += "<p>";
				}
				if (typeof data.url == "string") {
					body += "<a href=\"" + data.url + "\"";
					if (typeof data.new_page == "boolean" && data.new_page) {
						body += " target=\"_blank\"";
					}
					body += ">";
				}
				body += "&raquo; " + data.name;
				if (typeof data.url == "string") {
					body += "</a>";
				}
				body += "</p>";
			}
			else {
				body += "<td class=\"empty\">";
			}
			body += "</td>";
		}
		body += "</tr>";
	}
	body = "<table>" + body + "</table>";

	/* Print the table */
	if (typeof container == "string") {
		$("#" + container).html(body);
	}
	else {
		$(container).html(body);
	}
}

function table_add(item)
{
	if (typeof table_add.merge_array == "undefined") {
		/* Helper function to merge 2 arrays and remove duplicates */
		table_add.merge_array = function(a, b) {
			for (var key in b) {
				if (a.indexOf(b[key]) == -1) {
					a.push(b[key]);
				}
			}
			return a;
		};
	}

	var item_default = {
		"id": [],
		"category": [],
		"type": "",
		"name": "",
		"description": "",
		"uri": [],
		"boards": [],
		"mcus": [],
		"quickstart" : [],
		"appnote": []
	};
	// Use the default value if the info is empty
	for (var key in item) {
		item_default[key] = item[key];
	}
	item = item_default;
	// Generate the item ID
	item_id = get_id_from_item(item);
	// Add the item to the main list
	if (typeof doxygen_item_list[item_id] != "undefined") {
		/* Merge the entries */
		/* MCUs */
		doxygen_item_list[item_id]["mcus"] = table_add.merge_array(
				doxygen_item_list[item_id]["mcus"], item["mcus"]);
		/* Category */
		doxygen_item_list[item_id]["category"] = table_add.merge_array(
				doxygen_item_list[item_id]["category"], item["category"]);
		/* URI */
		for (var key in item["uri"]) {
			doxygen_item_list[item_id]["uri"][key] = item["uri"][key];
		}
		/* Boards */
		doxygen_item_list[item_id]["boards"] = table_add.merge_array(
				doxygen_item_list[item_id]["boards"], item["boards"]);
		/* Quickstarts */
		doxygen_item_list[item_id]["quickstart"] = table_add.merge_array(
				doxygen_item_list[item_id]["quickstart"], item["quickstart"]);
		/* Application Notes */
		doxygen_item_list[item_id]["appnote"] = table_add.merge_array(
				doxygen_item_list[item_id]["appnote"], item["appnote"]);
	}
	else
		doxygen_item_list[item_id] = item;
}

function get_id_from_item(item)
{
	var string_id = item["id"];
	// Generates a hash code from the string
	var hash = 0;
	for (i=0; i<string_id.length; i++) {
		c = string_id.charCodeAt(i);
		hash = hash * 31 + c;
		hash = hash & hash;
	}
	return hash;
}

function get_item_list()
{
	return doxygen_item_list;
}

function panel_expand(object)
{
	panel_retract();
	// Clone the description object
	var instance = $(jq_desc_selector).clone(true);
	// Change the ID of the instance
	$(instance).attr("id", jq_desc_selector.substr(1) + "_instance");
	// Add the new container
	var colspan_value = 1;
        if (display_column_type) colspan_value++;
	if (display_column_category) colspan_value++;
	if (display_column_device) colspan_value++;
	if (display_column_quickstart) colspan_value++;
	if (display_column_appnote) colspan_value++;
	if (display_column_board) colspan_value++;
        var panel_body = "<tr class=\"no_hover\">";
	panel_body += "<td id=\"id_panel_instance\" colspan=\"" + colspan_value + "\" style=\"width: auto;\"></td>";
	panel_body += "</tr>";
	$(object).after(panel_body);
	// Insert the new instance where it belongs
	$("#id_panel_instance").append(instance);
	// Display the instance
	instance.fadeIn(600);
}

function panel_retract()
{
	$("#id_panel_instance").parent().remove();
}

function item_expand(object, index)
{
	/* Remove the selection of any selected items from the table */
	$(jq_table_selector).find("tr").removeClass("selected");

	// Check if the panel is already opened for this object
	if ($(object).next("tr").get(0)
			&& $(object).next("tr").get(0) == $("#id_panel_instance").parent("tr").get(0)) {
		panel_retract();
		return;
	}

	// Expand the panel if needed
	panel_expand(object);
	/* Add a new selection */
	$(object).addClass("selected");
	var item = doxygen_item_list[index];
	// Update the title
	$(jq_desc_selector + "_instance").children("div.title").html(item["name"]);
	// Update the content (description)
        $(jq_desc_selector + "_instance").children("div.content").hide();
	if (item["description"]) {
                $(jq_desc_selector + "_instance").children("div.content").show();
		$(jq_desc_selector + "_instance").children("div.content").find("p").html(item["description"]);
	}
	// Update the links
	var jq_link = $(jq_desc_selector + "_instance").find("#id_arch_documentation").get(0);
	$(jq_link).find("div").empty();
	$(jq_link).hide();
        var table_data = new Array();
	for (var arch in item["uri"]) {
		$(jq_link).show();
                table_data.push({
			'name': asf_arch_to_title[arch],
			'url': item["uri"][arch]
		});
	}
        table_print(table_data, $(jq_link).find("div"), Math.ceil(table_data.length / 5), true);

	// Update the quickstart links if any
        var jq_link = $(jq_desc_selector + "_instance").find("#id_quickstart_documentation").get(0);
        $(jq_link).find("tbody").empty();
        $(jq_link).hide();
	var quickstart_table = new Array();
	for (var arch in item["quickstart"]) {
		var quickstart = item["quickstart"][arch];
		for (var id in quickstart) {
			var i = 0;
			result = quickstart[id].match(/use_case_([0-9]+)/);
			if (result) {
				i =  result[1];
			}
                        if (typeof quickstart_table[i] == "undefined") {
				quickstart_table[i] = new Array();
			}
			quickstart_table[i][arch] = quickstart[id];
		}
	}

	for (var i in quickstart_table) {
                $(jq_link).show();
		var row_html = "<tr><td>";
		if (i > 0) {
			row_html += "&raquo; Advanced Use Case #" + i;
		}
		else {
			row_html += "&raquo; Quick Start";
		}
		row_html += " (";
		for (var arch in quickstart_table[i]) {
			if (row_html.substr(-1) != "(") {
				row_html += ", ";
			}
                        row_html += "<a href=\"" + quickstart_table[i][arch] + "\">" + asf_arch_to_title[arch] + "</a>";
		}
                row_html += ")</td></tr>";
                $(jq_link).find("tbody").append(row_html);
	}

	// Update the application note links if any
        var jq_link = $(jq_desc_selector + "_instance").find("#id_appnote_documentation").get(0);
        $(jq_link).find("div").empty();
        $(jq_link).hide();
        var table_data = new Array();
	for (var id in item["appnote"]) {
		table_data.push({
			'name': item["appnote"][id]["name"],
			'url': item["appnote"][id]["link"],
			'new_page': true
		});
		$(jq_link).show();
	}
	table_print(table_data, $(jq_link).find("div"), 1, true);

	// Report a bug
	var jq_link = $(jq_desc_selector + "_instance").find("#id_links").get(0);
	$(jq_link).find("div").empty();
	var table_data = new Array(
		{
			'name': "Report a bug",
			'url': "javascript:report_bug(" + index + ");"
		},
		{
			'name': "Suggest an enhancement",
			'url': "javascript:suggest_enhancement(" + index + ");"
		}
	);
	table_print(table_data, $(jq_link).find("div"), 1);

	// Bug / enhancement list
	var jq_link = $(jq_desc_selector + "_instance").find("#id_bug_list").get(0);
	$(jq_link).hide();
	$(jq_link).find("div").empty();
	load_bugzilla_feed("http://asf.atmel.com/bugzilla/buglist.cgi?query_format=advanced&bug_status=NEW&bug_status=ASSIGNED&bug_status=REOPENED&bug_status=RESOLVED&bug_status=CLOSED&longdesc_type=allwordssubstr&longdesc=" + encodeURIComponent(item["id"].replace(/#.*/, "")),
			$(jq_link).find("div").get(0), 1, false, function(table_data) {
				if (table_data.length) {
					$(jq_link).show();
				}
			});
}

function page_update(item_list)
{
	category_update(item_list);
	type_update(item_list);
	mcus_update(item_list);
	kits_update(item_list);
	table_update(item_list);
}

function table_update(item_list)
{
	if (typeof table_update.is_first == "undefined")
		table_update.is_first = true;

	// Clear the content of the table
	$(jq_table_selector).children("tbody").empty();
	// Update the content of the table
	for (var key in item_list) {
		var category_list = "";
		for (var cat_key in item_list[key]["category"])
			category_list += ((category_list)?", ":"") + item_list[key]["category"][cat_key];
		var board_list = "";
		for (var board_key in item_list[key]["boards"])
			board_list += ((board_list)?", ":"") + item_list[key]["boards"][board_key];
		var part_list = "";
		for (var mcu_key in item_list[key]["mcus"])
			part_list += ((part_list)?", ":"") + item_list[key]["mcus"][mcu_key];
		var is_quickstart = false;
		for (var arch in item_list[key]["quickstart"]) {
			is_quickstart = true;
			break;
		}
		var row = "<tr onclick=\"javascript:item_expand(this, '" + key + "');\">"
				+ "<td>" + category_list + "</td>"
				+ "<td>" + item_list[key].type + "</td>"
				+ "<td>" + item_list[key].name + "</td>"
				+ "<td>" + part_list + "</td>"
				+ "<td>" + ((is_quickstart)?"X":"") + "</td>"
				+ "<td>" + ((item_list[key]["appnote"].length)?"X":"") + "</td>"
				+ "<td>" + board_list + "</td>"
				+ "</tr>";
		$(jq_table_selector).children("tbody").append(row);
	}

	/* Hide the category column */
	if (!display_column_category) {
		$(jq_table_selector).find("tr > th:nth-child(1)").hide();
		$(jq_table_selector).find("tr > td:nth-child(1)").hide();
	}
	/* Hide the type column */
	if (!display_column_type) {
		$(jq_table_selector).find("tr > th:nth-child(2)").hide();
		$(jq_table_selector).find("tr > td:nth-child(2)").hide();
	}
	/* Hide the device supported column */
	if (!display_column_device) {
		$(jq_table_selector).find("tr > th:nth-child(4)").hide();
		$(jq_table_selector).find("tr > td:nth-child(4)").hide();
	}
	/* Hide the quickstart column */
	if (!display_column_quickstart) {
		$(jq_table_selector).find("tr > th:nth-child(5)").hide();
		$(jq_table_selector).find("tr > td:nth-child(5)").hide();
	}
	/* Hide the application notes column */
	if (!display_column_appnote) {
		$(jq_table_selector).find("tr > th:nth-child(6)").hide();
		$(jq_table_selector).find("tr > td:nth-child(6)").hide();
	}
	/* Hide the board column */
	if (!display_column_board) {
		$(jq_table_selector).find("tr > th:nth-child(7)").hide();
		$(jq_table_selector).find("tr > td:nth-child(7)").hide();
	}

	if (table_update.is_first)
	{
		var tablesorter_options = {
			widthFixed: false,
			widgets: ['zebra']
		};
		/* Hack to bypass a IE bug */
		if (navigator.appName != 'Microsoft Internet Explorer') {
			tablesorter_options['sortList'] = sorting_method;
		}
		// Sort the table
		$(jq_table_selector).tablesorter(tablesorter_options).bind("sortEnd", function(sorter) {
			sorting_method = [sorter.target.config.sortList];
		});
	}
	else
	{
		$(jq_table_selector).trigger("update");
		// The update is triggered on a timeout of 1ms. If we want the appandCache + sorton event to start after,
		// we need to also trigger them on a timeout
		setTimeout(function() {
				$(jq_table_selector).trigger("appendCache");
				$(jq_table_selector).trigger("sorton", sorting_method);
		}, 1);
	}

	// Update the is_first attribute for next calls
	table_update.is_first = false;
}

function category_update(item_list)
{
	if (!$(jq_category_selector).get(0)) {
		return;
	}
	// Reset the style of the items in the droplist
	$(jq_category_selector).children('option:not(:first)').addClass('disabled');
	// Make a list of all available Kits
	for (var key in item_list) {
		for (var cat_key in item_list[key]["category"]) {
			var value = item_list[key]["category"][cat_key];
			$(jq_category_selector).children('option[value="' + value + '"]').removeClass('disabled');
		}
	}
}

function category_init(item_list)
{
	var category_list = new Array();
	// Make a list of all available Kits
	for (var key in item_list)
		for (var cat_key in item_list[key]["category"])
			category_list.push(item_list[key]["category"][cat_key]);
	// Update the select box
	helper_update_select(category_list, jq_category_selector);
}

function type_update(item_list)
{
	if (!$(jq_type_selector).get(0)) {
		return;
	}
	// Reset the style of the items in the droplist
	$(jq_type_selector).children('option:not(:first)').addClass('disabled');
	// Make a list of all available Kits
	for (var key in item_list) {
		var value = item_list[key]["type"];
		$(jq_type_selector).children('option[value="' + value + '"]').removeClass('disabled');
	}
}

function type_init(item_list)
{
	var type_list = new Array();
	// Make a list of all available Kits
	for (var key in item_list)
		type_list.push(item_list[key]["type"]);
	// Update the select box
	helper_update_select(type_list, jq_type_selector);
}

function mcus_update(item_list)
{
	if (!$(jq_mcu_selector).get(0)) {
		return;
	}
	// Reset the style of the items in the droplist
        $(jq_mcu_selector).find('option:not(:first)').addClass('disabled');
	// Make a list of all available Kits
	for (var key in item_list) {
		for (var mcu_key in item_list[key]["mcus"]) {
			var value = item_list[key]["mcus"][mcu_key];
			$(jq_mcu_selector).find('option[value="' + value + '"]').removeClass('disabled');
		}
	}

	/* Enable all the parent of the enabled items */
	level = 10;
	while (--level) {
		$(".level_" + level + ":not(.disabled)").prev(".level_" + (level - 1) + ".disabled").removeClass('disabled');
	}
}

function mcus_init_rec(list, display_list, class_list, mcu_list, indent, level)
{
	for (var mcu in mcu_list) {
		/* Add the current MCU to the list */
		list.push(mcu);
                display_list.push(indent + mcu);
                class_list.push("level_" + level);
		/* Add Children MCUs to the list as well */
		mcus_init_rec(list, display_list, class_list, mcu_list[mcu], indent + "&nbsp;", level + 1);
	}
}

function mcus_init(item_list)
{
        var list = new Array();
	var class_list = new Array();
	var display_list = new Array();

	// Make a list of all available MCUs
	mcus_init_rec(list, display_list, class_list, asf_mcu_arch, "", 0);

	// Update the select box
	helper_update_select(list, jq_mcu_selector, true, class_list, display_list);
}

function kits_update(item_list)
{
	if (!$(jq_kit_selector).get(0)) {
		return;
	}
	// Reset the style of the items in the droplist
        $(jq_kit_selector).children('option:not(:first)').addClass('disabled');
	// Make a list of all available Kits
	for (var key in item_list) {
		for (var kit_key in item_list[key]["boards"]) {
			var value = item_list[key]["boards"][kit_key];
			$(jq_kit_selector).children('option[value="' + value + '"]').removeClass('disabled');
		}
	}
}

function kits_init(item_list)
{
	var kit_list = new Array();
	// Make a list of all available Kits
	for (var key in item_list)
		for (var kit_key in item_list[key]["boards"])
			kit_list.push(item_list[key]["boards"][kit_key]);
	// Update the select box
	helper_update_select(kit_list, jq_kit_selector);
}

function helper_update_select(list, jq_selector, is_sorted, class_list, display_list)
{
	/* By default, assume that the list is not sorted */
	if (typeof is_sorted == "undefined") {
                is_sorted = false;
	}
	/* If the list is not sorted, then sort it! */
	if (!is_sorted) {
		// Sort the list
		list = list.sort();
	}
	// Remove duplicates
	var last = "";
	for (var key in list) {
		if (last == list[key])
			delete list[key];
		else
			last = list[key];
	}

	// Save the previous selection
	var previous_val = $(jq_selector).val();
	// Print the list
	$(jq_selector).empty();
	var selector_body = "<option value=\"all\">All</option>";
	var nb_elements = 0;
	for (var key in list) {
		var display = list[key];
		var value = list[key];
		if (typeof class_list != "undefined") {
			value += "\" class=\"" + class_list[key];
		}
		if (typeof display_list != "undefined") {
			display = display_list[key];
		}
		selector_body += "<option value=\"" + value + "\">" + display + "</option>";
		nb_elements++;
	}
        $(jq_selector).append(selector_body);
	// Select the previous value if it can
	$(jq_selector).val(previous_val);
	/* If the list has less than 2 elements, remove it */
	if (nb_elements <= 1) {
		$(jq_selector).parent().remove();
	}
}

function apply_search_filter(item_list)
{
	// Search filter
	var value = $(jq_search_selector).val();
	if (value != "") {
		value = value.toLowerCase();
		var filtered_list = new Array();
		for (var i in item_list) {
			// Search in the name
			if (item_list[i]["name"].toLowerCase().search(value) != -1)
				filtered_list[i] = item_list[i];
			else {
				// Search in the boards
				for (var j in item_list[i]["boards"]) {
					if (item_list[i]["boards"][j].toLowerCase().search(value) != -1) {
						filtered_list[i] = item_list[i];
						break;
					}
				}
				if (typeof filtered_list[i] == "undefined") {
					// Search in the parts
					for (var j in item_list[i]["mcus"]) {
						if (item_list[i]["mcus"][j].toLowerCase().search(value) != -1) {
							filtered_list[i] = item_list[i];
							break;
						}
					}
				}
				if (typeof filtered_list[i] == "undefined") {
					// Search in the categories
					for (var j in item_list[i]["category"]) {
						if (item_list[i]["category"][j].toLowerCase().search(value) != -1) {
							filtered_list[i] = item_list[i];
							break;
						}
					}
				}
			}
		}
		item_list = filtered_list;
	}
	return item_list;
}

function apply_filter(item_list, string_selector, jq_selector)
{
	var new_item_list = new Array();
	var value = $(jq_selector).val();

	if (value == "all" || !value) {
		return item_list;
	}

	if (typeof mcu_search_table[value] == "string") {
		value = "(" + mcu_search_table[value] + ")";
	}

        var rx = new RegExp("'\(-*[0-9]+\)[^']*@" + string_selector + value + "@", "g");

        while (result = rx.exec(search_string)) {
		id = result[1];
		if (typeof item_list[id] != "undefined") {
                        new_item_list[id] = item_list[id];
		}
	}

	return new_item_list;
}

function apply_category_filter(item_list)
{
	return apply_filter(item_list, "c", jq_category_selector);
}

function apply_type_filter(item_list)
{
	return apply_filter(item_list, "t", jq_type_selector);
}

function apply_device_filter(item_list)
{
	return apply_filter(item_list, "m", jq_mcu_selector);
}

function apply_board_filter(item_list)
{
	return apply_filter(item_list, "b", jq_kit_selector);
}

/*! \brief To speed up the research and the filtering, this function will
 * generate a string from the item_list.
 * The string will have the following format:
 * 'ID@cCATEGORY@tTYPE@mMCU@bBOARD@'
 * \param item_list The item list
 */
function build_search_string_from_item_list(item_list)
{
	var str = "";

        for (var id in item_list) {
		str += "'" + id;
                str += "@t" + item_list[id]["type"];
		for (var cat_id in item_list[id]["category"]) {
			str += "@c" + item_list[id]["category"][cat_id];
		}
		for (var mcu_id in item_list[id]["mcus"]) {
			str += "@m" + item_list[id]["mcus"][mcu_id];
		}
		for (var board_id in item_list[id]["boards"]) {
			str += "@b" + item_list[id]["boards"][board_id];
		}
		str += "@'";
	}

	return str;
}

/*! \brief This will create a search string table to speed up to quickly
 * identify the MCU groups related to the re-search.
 * \param mcu_arch An object representing the MCU architecture
 */
function build_search_table_from_mcu_arch(mcu_arch, mcu_above)
{
	var mcu_bellow = new Array();

	if (typeof mcu_above == "undefined") {
                mcu_above = new Array();
	}

	for (var group_name in mcu_arch) {
		/* Interate the process with the child node.
		 * mcu_bellow_group contains the list of all MCUs located bellow
		 * the group_name node
		 */
		var mcu_bellow_group = build_search_table_from_mcu_arch(
				mcu_arch[group_name], mcu_above.concat(Array(group_name)));
		/* Add the current group to the list */
                mcu_bellow_group.push(group_name);
		/* Generate the list of mcus for this node and add it to the main
		 * list.
		 */
		var mcu_search_string = "";
                var temp_group = mcu_bellow_group.concat(mcu_above);
		for (var i in temp_group) {
			if (mcu_search_string != "") {
                                mcu_search_string += "|";
			}
			mcu_search_string += temp_group[i];
		}
		/* If this is the first entry for group_name */
		if (typeof mcu_search_table[group_name] == "undefined") {
			mcu_search_table[group_name] = mcu_search_string;
		}
		/* If the entry already exists means there are at least 2 MCUs
		 * with the same name, then merge. Example: UC3B for UCR1 and
		 * UC3B for UCR2.
		 */
                else {
			mcu_search_table[group_name] += "|" + mcu_search_string;
		}
		/* Merge the array with the main one to get a list of all MCUs
		 * located bellow the node being processed
		 */
		mcu_bellow = mcu_bellow.concat(mcu_bellow_group);
	}
	return mcu_bellow;
}

function table_filter()
{
	var item_list = get_item_list();

	/* Apply the search filter */
	var item_list = apply_search_filter(item_list);

	/* Apply category filter */
	temp_item_list = apply_category_filter(item_list);
	/* Apply type filter */
	temp_item_list = apply_type_filter(temp_item_list);
	/* Apply device filter */
	temp_item_list = apply_device_filter(temp_item_list);

        kits_update(temp_item_list);

	/* Apply board filter */
	item_list = apply_board_filter(item_list);
	/* Apply category filter */
	temp_item_list = apply_category_filter(item_list);
	/* Apply type filter */
	temp_item_list = apply_type_filter(temp_item_list);

        mcus_update(temp_item_list);

	/* Apply device filter */
	item_list = apply_device_filter(item_list);
	/* Apply category filter */
	temp_item_list = apply_category_filter(item_list);

	type_update(temp_item_list);

	/* Apply type filter */
	item_list = apply_type_filter(item_list);

        category_update(temp_item_list);

	/* Apply category filter */
	item_list = apply_category_filter(item_list);

    table_update(item_list);
}

