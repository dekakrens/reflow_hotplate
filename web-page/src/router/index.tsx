import Main from "../pages/main";
import Settings from "../pages/settings";

const routes = [
  {
    path: "/",
    element: <Main />,
  },
  {
    path: "settings",
    element: <Settings />,
  },
];

export default routes;
