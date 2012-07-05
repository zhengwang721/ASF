/*! \brief This function will replace the doxygen menu by a custom menu.
 * \param select_menu The selection of the main menu
 * \param select_sub_menu The selection of the sub-menu
 * \param is_doxygen_documentation Defines if the documentation is a doxygen
 * documentation or a documentation mainpage.
 */
function customize_doxygen_menu(select_menu, is_doxygen_documentation)
{
	var body = "";

	var root_dir = "";
	if (is_doxygen_documentation) {
		root_dir = "../../";
	}

	var menu_items = {
/*		"top" : {
			"name": "Menu",
			"url": "",
			"level": 0
		},*/
		"home" : {
			"name": "Home",
			"url": root_dir + "index.html",
			"level": 1
		},
		"features" : {
			"name": "Features",
			"url": "",
			"level": 1
		},
		"asf_boards" : {
			"name": "ASF-Boards",
			"url": root_dir + "asf_boards.html",
			"level": 2
		},
		"asf_can_lin" : {
			"name": "ASF-CAN/LIN",
			"url": root_dir + "asf_can_lin.html",
			"level": 2
		},
		"asf_components" : {
			"name": "ASF-Components",
			"url": root_dir + "asf_components.html",
			"level": 2
		},
		"asf_drivers" : {
			"name": "ASF-Drivers",
			"url": root_dir + "asf_drivers.html",
			"level": 2
		},
		"asf_dsp" : {
			"name": "ASF-DSP",
			"url": root_dir + "asf_dsp.html",
			"level": 2
		},
		"asf_fs" : {
			"name": "ASF-FS",
			"url": root_dir + "asf_fs.html",
			"level": 2
		},
		"asf_gfx" : {
			"name": "ASF-GFX",
			"url": root_dir + "asf_gfx.html",
			"level": 2
		},
		"asf_sensors" : {
			"name": "ASF-Sensors",
			"url": root_dir + "asf_sensors.html",
			"level": 2
		},
		"asf_services" : {
			"name": "ASF-Services",
			"url": root_dir + "asf_services.html",
			"level": 2
		},
		"asf_usb" : {
			"name": "ASF-USB",
			"url": root_dir + "asf_usb.html",
			"level": 2
		},
		"3rd_party" : {
			"name": "3rd Party",
			"url": root_dir + "3rd_party.html",
			"level": 2
		},
		"cmsis" : {
			"name": "CMSIS",
			"url": root_dir + "cmsis.html",
			"level": 2
		},
		"documentation" : {
			"name": "Documentation",
			"url": "",
			"level": 1
		},
		"architecture" : {
			"name": "ASF Architecture",
			"url": root_dir + "architecture.html",
			"level": 2
		},
		"reference_manual" : {
			"name": "Reference Manual",
			"url": "http://www.atmel.com/dyn/resources/prod_documents/doc8432.pdf",
			"level": 2
		},
		"api" : {
			"name": "API",
			"url": root_dir + "api.html",
			"level": 2
		},
		"applications" : {
			"name": "Applications",
			"url": root_dir + "applications.html",
			"level": 2
		},
		"unit_tests" : {
			"name": "Unit-Tests",
			"url": root_dir + "unit_tests.html",
			"level": 2
		},
		"release_notes" : {
			"name": "Release Notes",
			"url": "http://www.atmel.com/asf",
			"level": 2
		},
		"getting_started" : {
			"name": "Get Started",
			"url": root_dir + "get_started.html",
			"level": 1
		},
		"download" : {
			"name": "Download",
			"url": root_dir + "download.html",
			"level": 1
		},
		"bug_tracker" : {
			"name": "Bug Tracker",
			"url": root_dir + "bug_tracker.html",
			"level": 1
		}
	}

	/* This block identifies the menu hierarchy and build a table for quick look-up */
	customize_doxygen_menu.build_menu_hierarchy = function(select_menu) {
		var menu_hierarchy = new Array();
		if (typeof menu_items[select_menu] != "undefined") {
			/* Detect the parent menu */
			var parent_menu = new Array();
			for (var menu_name in menu_items) {
				var menu = menu_items[menu_name];
				/* Save the parents */
				parent_menu[menu["level"]] = menu_name;
				/* Exit the loop if the item has been found */
				if (menu_name == select_menu) {
					break;
				}
			}
			/* Generate the parent menu hierarchy */
			for (var i in parent_menu) {
				var menu_name = parent_menu[i];
				if (menu_items[menu_name]["level"] <= menu_items[select_menu]["level"]) {
					menu_hierarchy[menu_name] = 1;
				}
			}
		}
		return menu_hierarchy;
	};

	/* This function selects a menu item */
	customize_doxygen_menu.select = function(select_menu) {
		/* Check if the current item is already selected */
		if ($("ul.tablist").find("div.menu_item_" + select_menu).hasClass("retracted")) {
			$("ul.tablist").find("div.menu_item_" + select_menu).removeClass("retracted");
			$("ul.tablist").find("div.menu_item_" + select_menu + " + div").animate({
				height: 'toggle'
			}, 300);
		}
		else {
			$("ul.tablist").find("div.menu_item_" + select_menu + " + div").animate({
				height: 'toggle'
			}, 300, function() { $(this).prev().addClass("retracted"); });
		}
	};

	/* Build the menu hierarchy */
	var menu_hierarchy = customize_doxygen_menu.build_menu_hierarchy(select_menu);

	var menu_to_retract = new Array();
	var previous_level = 0;
	for (var menu_name in menu_items) {
		var menu = menu_items[menu_name];

		/* Set a div hierarchy to ease CSS rules */
		if (previous_level < menu["level"]) {
			body += "<div class=\"menu_container\">";
		}
		else if (previous_level > menu["level"]) {
			body += "</div>";
		}
		previous_level = menu["level"];

		/* Set the type ofthe menu (=level) */
		body += "<div class=\"";
		if (menu["level"] == 0) {
			body += "menu_top";
		}
		else if (menu["level"] == 1) {
			body += "menu_item";
			/* Expand the menu by default if no sub-category is selected */
			if (typeof menu_items[select_menu] != "undefined") {
				if (menu_items[select_menu]["level"] > 1 && !(menu_name in menu_hierarchy)) {
					menu_to_retract.push(menu_name);
				}
			}
		}
		else if (menu["level"] == 2) {
			body += "menu_item_sub";
		}
		/* Add a specific class to identify the menu */
		body += " menu_item_" + menu_name;
		/* Select the item if needed */
		if (menu_name in menu_hierarchy) {
			body += " menu_item_selected";
		}
		/* Tell if this item has no link */
		if (!menu_items[menu_name]["url"]) {
			body += " no_link\" onclick=\"javascript:customize_doxygen_menu.select('" + menu_name + "');\"";
		}
		else {
			body += "\"><a href=\""
					+ menu_items[menu_name]["url"]
					+ "\"";
		}
		body += ">" + menu_items[menu_name]["name"];
		if (menu_items[menu_name]["url"]) {
			body += "</a>";
		}
		body += "</div>";
	}

	/* Remove the current menu */
	$("ul.tablist").children('*:not(#searchli)').remove();
	/* Replace it with the new one */
	$("ul.tablist").prepend(body);

	/* Retract some menus */
	for (var i in menu_to_retract) {
		$("div.menu_item_" + menu_to_retract[i] + " + div.menu_container").animate({ height: 'toggle' }, 0);
                $("div.menu_item_" + menu_to_retract[i]).addClass("retracted");
	}
}

/*! \brief This function will change a page behaviour.
 * Instead of printing all the symbol definitons and declarations on the same
 * page, it will display either all the declatations or only one specific
 * definition.
 */
function customize_doxygen_layout()
{
	/* This function will display a symbol definition.
	 * It will parse the url to determine which definition to display.
	 * \param url The page URL
	 */
	var switch_to_def = function (url) {
		var hash = url.split("#");
		/* If the URL contains a hash */
		if (typeof hash[1] != "undefined") {
			/* If there is no content for this symbol definition or
			 * if the anchor does not exists
			 */
			if (!$('#' + hash[1]).next().html()) {
				return;
			}
			/* Hide all the content */
			$('.contents').children('*').hide();
			/* Create the HTML code of the back link */
			var back_link = "[<a href=\"" + hash[0] + "\" "
					+ "class=\"el\">Declarations</a>]";
			/* Remove all the previous links */
			$('.headertitle h1 br:first').nextAll('*')
					.add('.headertitle h1 br:first')
					.remove('*');
			/* Display our custom link */
                        $('.headertitle h1').append("<br /><small>" + back_link + "</small>");
			/* Display the item */
			$('#' + hash[1]).next().show();
		}
	}
	/* Call the swith_to_def function when the page loads */
        if (window.location.hash) {
		switch_to_def(window.location.hash);
	}
	/* Call the swith_to_def function each time the user clicks on a link */
	$('a').click(function() {
		switch_to_def($(this).attr('href'));
	});
}

/*! \brief This function will customize the search button of a doxygen page.
 */
function customize_doxygen_search()
{
	/* Customize the search button */
	$('#MSearchBox .right').hover(function () {
			searchBox.OnSearchSelectShow();
		},
		function () {
			searchBox.OnSearchSelectHide();
		}
	);
}

/*! \brief This function returns the menu selection by parsing the current URL.
 * \return Menu selection ID (\see \ref customize_doxygen_menu for values)
 */
function get_menu_selection(is_doxygen)
{
	var url = window.location;
	var select_menu;
	var url_regexpr_menu;

	if (is_doxygen) {
		url_regexpr_menu = {
			"applications": "related__projects.html",
			"unit_tests": "unit__tests.html",
			"api": ".*" //< default value must be at the end of the table
		};
	}
	else {
		url_regexpr_menu = {
			"asf_sensors": "\/asf_sensors.html",
			"asf_boards": "\/asf_boards.html",
			"asf_drivers": "\/asf_drivers.html",
			"asf_services": "\/asf_services.html",
			"asf_components": "\/asf_components.html",
			"cmsis": "\/cmsis.html",
			"3rd_party": "\/3rd_party.html",
			"asf_usb": "\/asf_usb.html",
			"asf_fs": "\/asf_fs.html",
			"asf_can_lin": "\/asf_can_lin.html",
			"asf_gfx": "\/asf_gfx.html",
			"asf_dsp": "\/asf_dsp.html",
			"architecture": "\/architecture.html",
			"applications": "\/applications.html",
			"unit_tests": "\/unit_tests.html",
			"getting_started": "\/get_started.html",
			"download": "\/download.html",
			"bug_tracker": "\/bug_tracker.html",
			"api": "\/(api.html|search.html)",
			"home": "\/index.html" //< default value must be at the end of the table
		};
	}

	for (select_menu in url_regexpr_menu) {
		var rx = new RegExp(url_regexpr_menu[select_menu]);
		if (rx.test(url)) {
			break;
		}
	}

	return select_menu;
}

var _gaq = _gaq || [];
function setup_google_analytics()
{
	_gaq.push(['_setAccount', 'UA-2587932-1']);
	_gaq.push(['_setDomainName', '.atmel.com']);
	_gaq.push(['_setAllowLinker', true]);
	_gaq.push(['_setAllowAnchor', true]);
	_gaq.push(['_trackPageLoadTime']);
	_gaq.push(['_trackPageview']);

	var ga = document.createElement('script');
	ga.type = 'text/javascript';
	ga.async = true;
	ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www')
			+ '.google-analytics.com/ga.js';
	var s = document.getElementsByTagName('script')[0];
	s.parentNode.insertBefore(ga, s);
}

/*! \brief This function will customize the doxygen page. It must be called when
 * the page has been loaded.
 */
function customize_doxygen()
{
	var select_menu = get_menu_selection(true);
	setup_google_analytics();
	customize_doxygen_menu(select_menu, true);
	customize_doxygen_layout();
	customize_doxygen_search();
}

/*! \brief This function will customize the mainpage of the documentation.
 * It must be called when the page has been loaded.
 */
function customize_mainpage()
{
	var select_menu = get_menu_selection(false);
	setup_google_analytics();
	customize_doxygen_menu(select_menu, false);
}
