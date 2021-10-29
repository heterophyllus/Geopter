
def load_triplet(gui):
    triplet_json_path = gui.app_dir() + '/example/triplet.json'
    gui.load_lens(triplet_json_path)


def analysis_all(gui):
    gui.prescription()
    gui.longitudinal()
    gui.transverse()
    gui.spot_diagram()

def run_demo(gui):
    load_triplet(gui)
    analysis_all(gui)

