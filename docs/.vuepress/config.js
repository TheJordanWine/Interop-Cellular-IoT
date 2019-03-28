module.exports = {
    title: 'Interop Cellular IoT',
    description: 'Documentions for Team 6 Project',
    markdown: {
        lineNumbers: true
    },
    port: 5000,
    host: 'localhost',
    themeConfig: {
        nav: [{
                text: 'Home',
                link: '/'
            },
            {
                text: 'Team 6',
                items: [{
                    text: 'Interop-Cellular-IoT Repo',
                    link: 'https://github.com/TheJordanWine/Interop-Cellular-IoT'
                },{
                    text: 'Interop-Cellular-IoT Taiga',
                    link: 'https://winehome.publicvm.com/project/jordanwine-interop-cellular-iot/'
                }, {
                    text: 'Interop-Cellular-IoT Google Drive',
                    link: 'https://drive.google.com/drive/u/0/folders/1G3z-eMDizR5JsMXnwcRMyJ6EEdeh1b18'
                }]
            },{
                text: 'External Docs',
                items: [{
                    text: 'OM2M Docs',
                    link: 'https://wiki.eclipse.org/OM2M'
                }, {
                    text: 'oneM2M Docs',
                    link: 'http://www.onem2m.org/developers-corner/documentation/getting-started'
                }]
            }
        ],
        sidebar: {
            '/': [
                ['', 'Introduction'], // Introduction points to README.md
                ['client.md', 'C++ Client'],
                ['client_c.md', 'C Client'],
                ['server.md', 'Server'],
                ['webapp.md', 'WebApp'],
                ['testing.md', 'Testing']
            ]
        }
    }
};
