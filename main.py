from SimcppGenerator import SimcppGenerator

generator = SimcppGenerator(pb=r"C:\Users\yiyan5ez\Desktop\SimcppGenerator\SimcppGenerator.pb",
                            pb_yaml=r"C:\Users\yiyan5ez\Desktop\SimcppGenerator\pcs.yaml",
                            ps_dir=r"D:\\Simcenter Prescan\\Prescan_2302\\lib\\cmake",
                            load_yaml=True)

generator.copy_to_project()
generator.generate_codes()
generator.write_to_project()
