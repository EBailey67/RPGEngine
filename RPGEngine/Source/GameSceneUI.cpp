#include "GameSceneUI.h"

#include "game_scene.hpp"

namespace GUI
{
	GameSceneUI::GameSceneUI(SDL_Window* pwindow, int rwidth, int rheight) :
		Screen(pwindow, Vector2i(rwidth, rheight), "Game UI Test")
	{

			{
				struct Ability
				{
					std::string name;
					int value;
				};

				struct Ability abilities[6] = 
                {
					{"Strength", 16},
					{"Intelligence", 11},
					{"Wisdom", 14},
					{"Dexterity", 6},
					{"Constitution", 18},
					{"Charisma", 13},
                };
				
				// Create the window
				auto& window = wdg<Window>("Abilities");
				window.withPosition({0, 0});

				// Create a 2 column layout
				auto* layout = new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 15, 5);
				layout->setColAlignment({Alignment::Maximum, Alignment::Fill});
				layout->setSpacing(0, 10);
				window.setLayout(layout);
					
                for (auto& ability : abilities)
                {
	                //window.add<Label>(abilitie.name, "sans-bold");
                	auto& label = window.wdg<Label>(ability.name, "sans-bold");
                	label.setFontSize(14);
	                auto& textBox = window.wdg<TextBox>();
	                textBox.setEditable(false);
	                textBox.setFixedSize(Vector2i(40, 20));
	                textBox.setValue(std::to_string(ability.value));
	                textBox.setFontSize(16);
                	textBox.setAlignment(TextBox::Alignment::Right);
	                
                }
			}
			{

				{
				// Create the window
				auto& window = wdg<Window>("Debug Information");
				window.withPosition({rwidth - 300, 0});

				// Create a 2 column layout
				auto* layout = new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 15, 5);
				layout->setColAlignment({Alignment::Maximum, Alignment::Fill});
				layout->setSpacing(0, 10);
				window.setLayout(layout);
					
                window.add<Label>("Framerate :", "sans-bold");
				auto& label = window.wdg<Label>("FPS");
				label.withId("FPS");

				window.add<Button>("Debug Overlay");
                }
			}

			{

				{
				// Create the window
				auto& window = wdg<Window>("Status").withLayout<GroupLayout>();
//				auto& window = wdg<Widget>().withLayout<GroupLayout>();
				window.withPosition({rwidth / 2 - 150, 0});
				

				// Health Bar

				auto& panel = window.wdg<Widget>();
                panel.setLayout(new BoxLayout(Orientation::Horizontal,
                    Alignment::Middle, 0, 6));

                panel.add<Label>("HP :", "sans-bold")->setFixedWidth(32);
				auto& healthBar = panel.wdg<ProgressBar>();

				healthBar.withId("HP");
				healthBar.setFixedWidth(200);
				healthBar.setValue(1.0f);
                 ref<Theme> t = new Theme();
				healthBar.setTheme(t);
				healthBar.theme()->mProgressBarTop = Color(192, 64, 64, 255);
				healthBar.theme()->mProgressBarBot = Color(96, 8, 8, 255);
					
                // Mana Bar
    //            auto& mana_panel = window.wdg<Widget>();
    //            mana_panel.setLayout(new BoxLayout(Orientation::Horizontal,
    //                Alignment::Middle, 0, 6));

				//mana_panel.add<Label>("MP :", "sans-bold")->setFixedWidth(32);
				//auto& manaBar = mana_panel.wdg<ProgressBar>();
				//manaBar.withId("MP");
				//manaBar.setFixedWidth(200);
				//manaBar.setValue(0.5f);
    //             ref<Theme> tMana = new Theme();
				//manaBar.setTheme(tMana);
				//manaBar.theme()->mProgressBarTop = Color(96, 96, 255, 255);
				//manaBar.theme()->mProgressBarBot = Color(32, 32, 96, 255);

                auto& score = window.label("Score : 0", "nimbusmono-oblique.otf", 16).withId("SCORE");
                auto& dash = window.label("Dash : Available", "sans-bold").withId("DASH");
									
                //auto& tools = window.widget().boxlayout(Orientation::Horizontal, Alignment::Middle, 0, 6);

                //tools.toolbutton(ENTYPO_ICON_CLOUD, "", 64, 64, Button::Flags::NormalButton)._and()
                //    .toolbutton(ENTYPO_ICON_FF, "", 64, 64, Button::Flags::NormalButton)._and()
                //    .toolbutton(ENTYPO_ICON_COMPASS, "", 64, 64, Button::Flags::NormalButton)._and()
                //    .toolbutton(ENTYPO_ICON_INSTALL, "", 64, 64, Button::Flags::NormalButton);

					
                }
			}
		
#if 0
	            {
                auto& nwindow = window("Button demo", Vector2i{ 15, 15 })
                    .withLayout<GroupLayout>();

                nwindow.label("Push buttons", "sans-bold")._and()
                    .button("Plain button", [] { std::cout << "pushed!" << std::endl; })
                    .withTooltip("This is plain button tips");

                nwindow.button("Styled", ENTYPO_ICON_ROCKET, [] { std::cout << "pushed!" << std::endl; })
                    .withBackgroundColor(Color(0, 0, 255, 25));

                nwindow.label("Toggle buttons", "sans-bold")._and()
                    .button("Toggle me", [](bool state) { std::cout << "Toggle button state: " << state << std::endl; })
                    .withFlags(Button::ToggleButton);

                nwindow.label("Radio buttons", "sans-bold")._and()
                    .button("Radio button 1")
                    .withFlags(Button::RadioButton);

                nwindow.button("Radio button 2")
                    .withFlags(Button::RadioButton)._and()
                    .label("A tool palette", "sans-bold");

                auto& tools = nwindow.widget().boxlayout(Orientation::Horizontal, Alignment::Middle, 0, 6);

                tools.toolbutton(ENTYPO_ICON_CLOUD)._and()
                    .toolbutton(ENTYPO_ICON_FF)._and()
                    .toolbutton(ENTYPO_ICON_COMPASS)._and()
                    .toolbutton(ENTYPO_ICON_INSTALL);

                nwindow.label("Popup buttons", "sans-bold")._and()
                    .popupbutton("Popup", ENTYPO_ICON_EXPORT)
                    .popup()
                    .withLayout<GroupLayout>()
                    .label("Arbitrary widgets can be placed here")._and()
                    .checkbox("A check box")._and()
                    .popupbutton("Recursive popup", ENTYPO_ICON_FLASH).popup()
                    .withLayout<GroupLayout>()
                    .checkbox("Another check box");
            }
#endif	
#if 0
            ListImages images = loadImageDirectory(SDL_GetRenderer(pwindow), "icons");

            {
                auto& pwindow = window("Basic widgets", Vector2i{ 200, 15 }).withLayout<GroupLayout>();

                pwindow.label("Message dialog", "sans-bold")._and()
                    .widget()
                    .boxlayout(Orientation::Horizontal, Alignment::Middle, 0, 6)
                    .button("Info", [&] {
                    msgdialog(MessageDialog::Type::Information, "Title", "This is an information message",
                        [](int result) { std::cout << "Dialog result: " << result << std::endl; }); })._and()
                        .button("Warn", [&] {
                        msgdialog(MessageDialog::Type::Warning, "Title", "This is a warning message",
                            [](int result) { std::cout << "Dialog result: " << result << std::endl; }); })._and()
                            .button("Ask", [&] {
                            msgdialog(MessageDialog::Type::Warning, "Title", "This is a question message",
                                "Yes", "No", true, [](int result) { std::cout << "Dialog result: " << result << std::endl; }); });

                        pwindow.label("Image panel & scroll panel", "sans-bold");
                        auto& imagePanelBtn = pwindow.popupbutton("Image Panel", ENTYPO_ICON_FOLDER);

                        // Load all of the images by creating a GLTexture object and saving the pixel data.
                        m_current_image = 0;
                        for (auto& icon : images) m_images_data.emplace_back(icon.tex);

                        auto& img_window = window("Selected image", Vector2i(675, 15));
                        img_window.withLayout<GroupLayout>();

                        auto imageView = img_window.add<ImageView>(m_images_data[0]);

                        imagePanelBtn.popup(Vector2i(245, 150))
                            .vscrollpanel()
                            .imgpanel(images)
                            .setCallback([this, imageView](int i)
                                {
                                    if (i >= m_images_data.size())
                                        return;
                                    imageView->bindImage(m_images_data[i]);
                                    m_current_image = i;
                                    std::cout << "Selected item " << i << '\n';
                                });


                        // Change the active textures.

                        imageView->setGridThreshold(20);
                        imageView->setPixelInfoThreshold(20);
                        imageView->setPixelInfoCallback(
                            [this, imageView](const Vector2i& index) -> std::pair<std::string, Color>
                            {
                                void* pixels;
                                int pitch, w, h;
                                SDL_QueryTexture(m_images_data[m_current_image], nullptr, nullptr, &w, &h);

                                SDL_LockTexture(m_images_data[m_current_image], nullptr, &pixels, &pitch);
                                auto* const imageData = static_cast<Uint32*>(pixels);

                                std::string stringData;
                                uint16_t channelSum = 0;
                                for (int i = 0; i != 4; ++i)
                                {
	                                auto data = reinterpret_cast<uint8_t*>(imageData);
                                    auto& channelData = data[4 * index.y * w + 4 * index.x + i];
                                    channelSum += channelData;
                                    stringData += (std::to_string(static_cast<int>(channelData)) + "\n");
                                }
                                float intensity = static_cast<float>(255 - (channelSum / 4)) / 255.0f;
                                float colorScale = intensity > 0.5f ? (intensity + 1) / 2 : intensity / 2;
                                Color textColor = Color(colorScale, 1.0f);
                                SDL_UnlockTexture(m_images_data[m_current_image]);
                                return { stringData, textColor };
                            }
                        );

                        pwindow.label("File dialog", "sans-bold")._and()
                            .widget()
                            .boxlayout(Orientation::Horizontal, Alignment::Middle, 0, 6)
                            .button("Open", [&] {
                            std::cout << "File dialog result: " << file_dialog(
                                { {"png", "Portable Network Graphics"}, {"txt", "Text file"} }, false) << std::endl;
                                })._and()
                                    .button("Save", [&] {
                                    std::cout << "File dialog result: " << file_dialog(
                                        { {"png", "Portable Network Graphics"}, {"txt", "Text file"} }, true) << std::endl;
                                        });

                                pwindow.label("Combo box", "sans-bold")._and()
                                    .dropdownbox(std::vector<std::string>{ "Dropdown item 1", "Dropdown item 2", "Dropdown item 3" })._and()
                                    .combobox(std::vector<std::string>{ "Combo box item 1", "Combo box item 2", "Combo box item 3"})._and()
                                    .label("Check box", "sans-bold")._and()
                                    .checkbox("Flag 1", [](bool state) { std::cout << "Check box 1 state: " << state << std::endl; })
                                    .withChecked(true)._and()
                                    .checkbox("Flag 2", [](bool state) { std::cout << "Check box 2 state: " << state << std::endl; })._and()
                                    .label("Progress bar", "sans-bold")._and()
                                    .progressbar().withId("progressbar")._and()
                                    .label("Slider and text box", "sans-bold")._and()
                                    .widget().withLayout<BoxLayout>(Orientation::Horizontal, Alignment::Middle, 0, 20)
                                    .slider(0.5f, [](Slider* obj, float value) {
                                    if (auto* textBox = obj->gfind<TextBox>("slider-textbox"))
                                        textBox->setValue(std::to_string((int)(value * 100)));
                                        }, [](float value) { std::cout << "Final slider value: " << (int)(value * 100) << std::endl; })
                                    .withFixedWidth(80)._and()
                                            .textbox("50", "%").withAlignment(TextBox::Alignment::Right)
                                            .withId("slider-textbox")
                                            .withFixedSize(Vector2i(60, 25))
                                            .withFontSize(20);

                                        pwindow.label("A switch boxes", "sans-bold");
                                        Widget* swbx = new Widget(&pwindow);
                                        swbx->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 2));

                                        auto* swbh = new SwitchBox(swbx, SwitchBox::Alignment::Horizontal, "");
                                        swbh->setFixedSize(Vector2i(84, 32));
                                        new SwitchBox(swbx, SwitchBox::Alignment::Vertical, "");
            }
#endif
#if 0	
            {
                auto& window = wdg<Window>("Misc. widgets");
                window.setPosition(425, 15);
                window.setFixedSize({ 400, 300 });
                window.setLayout(new GroupLayout());
                TabWidget* tabWidget = window.add<TabWidget>();

                Widget* layer = tabWidget->createTab("Color Wheel");
                layer->setLayout(new GroupLayout());

                // Use overloaded variadic add to fill the tab widget with Different tabs.
                layer->add<Label>("Color wheel widget", "sans-bold");
                ColorWheel* colorwheel = layer->add<ColorWheel>();
                colorwheel->setCallback([colorwheel](const Color& value) 
					{
                	colorwheel->setColor(value);
                	colorwheel->dirty();
                    });


                layer = tabWidget->createTab("Function Graph");
                layer->setLayout(new GroupLayout());

                layer->add<Label>("Function graph widget", "sans-bold");

                Graph* graph = layer->add<Graph>("Some Function");

                graph->setHeader("E = 2.35e-3");
                graph->setFooter("Iteration 89");
                std::vector<float>& func = graph->values();
                func.resize(100);
                for (int i = 0; i < 100; ++i)
                    func[i] = 0.5f * (0.5f * std::sin(i / 10.f) +
                        0.5f * std::cos(i / 23.f) + 1);

                // Dummy tab used to represent the last tab button.
                tabWidget->createTab("+");

                // A simple counter.
                int counter = 1;
                tabWidget->setCallback([tabWidget, this, counter](int index) mutable {
                    if (index == (tabWidget->tabCount() - 1)) {
                        // When the "+" tab has been clicked, simply add a new tab.
                        std::string tabName = "Dynamic " + std::to_string(counter);
                        Widget* layerDyn = tabWidget->createTab(index, tabName);
                        layerDyn->setLayout(new GroupLayout());
                        layerDyn->add<Label>("Function graph widget", "sans-bold");
                        Graph* graphDyn = layerDyn->add<Graph>("Dynamic function");

                        graphDyn->setHeader("E = 2.35e-3");
                        graphDyn->setFooter("Iteration " + std::to_string(index * counter));
                        std::vector<float>& funcDyn = graphDyn->values();
                        funcDyn.resize(100);
                        for (int i = 0; i < 100; ++i)
                            funcDyn[i] = 0.5f *
                            std::abs((0.5f * std::sin(i / 10.f + counter) +
                                0.5f * std::cos(i / 23.f + 1 + counter)));
                        ++counter;
                        // We must invoke perform layout from the screen instance to keep everything in order.
                        // This is essential when creating tabs dynamically.
                        performLayout();
                        // Ensure that the newly added header is visible on screen
                        tabWidget->ensureTabVisible(index);

                    }
                    });
                tabWidget->setActiveTab(0);

                // A button to go back to the first tab and scroll the window.
                auto& panel = window.wdg<Widget>();
                panel.add<Label>("Jump to tab: ");
                panel.setLayout(new BoxLayout(Orientation::Horizontal,
                    Alignment::Middle, 0, 6));

                auto ib = panel.add<IntBox<int>>();
                ib->setEditable(true);

                auto b = panel.add<Button>("", ENTYPO_ICON_FORWARD);
                b->setFixedSize(Vector2i(22, 22));
                ib->setFixedHeight(22);
                b->setCallback([tabWidget, ib] {
                    int value = ib->value();
                    if (value >= 0 && value < tabWidget->tabCount()) {
                        tabWidget->setActiveTab(value);
                        tabWidget->ensureTabVisible(value);
                    }
                    });
            }
#endif
#if 0	
            {
                auto& window = wdg<Window>("Grid of small widgets");
                window.withPosition({ 425, 288 });
                auto* layout = new GridLayout(Orientation::Horizontal, 2,
                    Alignment::Middle, 15, 5);
                layout->setColAlignment({ Alignment::Maximum, Alignment::Fill });
                layout->setSpacing(0, 10);
                window.setLayout(layout);
           	
                window.add<Label>("Floating point :", "sans-bold");
                auto& textBox = window.wdg<TextBox>();
                textBox.setEditable(true);
                textBox.setFixedSize(Vector2i(100, 20));
                textBox.setValue("50");
                textBox.setUnits("GiB");
                textBox.setDefaultValue("0.0");
                textBox.setFontSize(16);
                textBox.setFormat("[-]?[0-9]*\\.?[0-9]+");

                window.add<Label>("Positive integer :", "sans-bold");
                auto& textBox2 = window.wdg<TextBox>();
                textBox2.setEditable(true);
                textBox2.setFixedSize(Vector2i(100, 20));
                textBox2.setValue("50");
                textBox2.setUnits("Mhz");
                textBox2.setDefaultValue("0.0");
                textBox2.setFontSize(16);
                textBox2.setFormat("[1-9][0-9]*");

                window.add<Label>("Checkbox :", "sans-bold");

                window.wdg<CheckBox>("Check me")
                    .withChecked(true)
                    .withFontSize(16);

                window.add<Label>("Combo box :", "sans-bold");
                window.wdg<ComboBox>()
                    .withItems(std::vector<std::string>{ "Item 1", "Item 2", "Item 3" })
                    .withFontSize(16)
                    .withFixedSize(Vector2i(100, 20));

                window.add<Label>("Color button :", "sans-bold");
                auto& popupBtn = window.wdg<PopupButton>("", 0);
                popupBtn.setBackgroundColor(Color(255, 120, 0, 255));
                popupBtn.setFontSize(16);
                popupBtn.setFixedSize(Vector2i(100, 20));
                auto& popup = popupBtn.popup().withLayout<GroupLayout>();

                ColorWheel& colorwheel = popup.wdg<ColorWheel>();
                colorwheel.setColor(popupBtn.backgroundColor());

                Button& colorBtn = popup.wdg<Button>("Pick");
                colorBtn.setFixedSize(Vector2i(100, 25));
                Color c = colorwheel.color();
                colorBtn.setBackgroundColor(c);

                colorwheel.setCallback([&colorBtn, &colorwheel](const Color& value) 
					{
                    colorBtn.setBackgroundColor(value);
                	
                	colorwheel.setColor(value);
                	colorwheel.dirty();
                    });

                colorBtn.setChangeCallback([&colorBtn, &popupBtn](bool pushed) {
                    if (pushed) {
                        popupBtn.setBackgroundColor(colorBtn.backgroundColor());
                        popupBtn.setPushed(false);
                    }
                    });
            }
#endif	
            Screen::performLayout(m_sdl_renderer);

	}

    void GameSceneUI::draw(SDL_Renderer* renderer)
    {
	    if (auto* pfps = gfind<Label>("FPS"))
        {
   			auto* game = Instances::GetGameInstance();
			auto* gs = dynamic_cast<GameScene*>(game->Scene());

            const auto frameRate = static_cast<int>(floorf(game->fps_counter.GetFrameRate() * 100 + 0.5f) / 100.0f);
	    	
        	pfps->setCaption(std::to_string(frameRate));
        }

	    const auto playerView = registry.view<Player, Hierarchy, Position, Health, Dash>();
		auto &&[player, hierarchy, health, pos, dash] = registry.get<Player, Hierarchy, Health, Position, Dash>(*playerView.begin());

        if (auto* pbar = gfind<ProgressBar>("HP"))
        {
            pbar->setValue(static_cast<float>(health.health) / 3.0f);
        }

		
        if (auto* plabel = gfind<Label>("SCORE"))
        {
            plabel->setCaption("Score :" + std::to_string(player.score));
        }

        if (auto* plabel = gfind<Label>("DASH"))
        {
        	if(dash.canDashing)
        	{
				plabel->setCaption("Dash Available");
        		plabel->setColor(Color(0, 192, 0, 255));
            }
        	else
            {
                plabel->setCaption("Dash NO");
        		plabel->setColor(Color(192, 0, 0, 255));
            }
        }

		
        Screen::draw(renderer);
    }


}