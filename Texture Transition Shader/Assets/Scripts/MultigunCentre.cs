using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MultigunCentre : MonoBehaviour {

    public float rotateSpeed;

    public GameObject[] ships;

	void Start () {
        EnableShip(2);
	}
	
	// Update is called once per frame
	void Update ()
    {
        Rotate();
	}

    void Rotate()
    {
        transform.Rotate(0, rotateSpeed * Time.deltaTime, 0);
    }

    public void EnableShip(int shipID)
    {
        foreach (var ship in ships)
        {
            ship.GetComponent<MeshRenderer>().enabled = false;
        }

        ships[shipID].GetComponent<MeshRenderer>().enabled = true;
    }
}
